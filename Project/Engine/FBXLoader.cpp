#include "PCH_Engine.h"
#include "FBXLoader.h"

#include "PathMgr.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Material.h"
#include "define_Util.h"



using namespace fbxsdk;
namespace mh
{
	
	

	FBXLoader::FBXLoader()
		: mManager()
		, mScene()
		, mBones{}
		, mAnimNames{}
		, mAnimClips{}
		, mbMixamo{}
	{
	}

	FBXLoader::~FBXLoader()
	{
		Reset();
	}

	void FBXLoader::Reset()
	{
		mContainers.clear();
		mBones.clear();

		for (int i = 0; i < mAnimNames.Size(); ++i)
		{
			if (mAnimNames[i])
			{
				mAnimNames[i]->Clear();
				delete mAnimNames[i];
			}
		}
		mAnimNames.Clear();

		mAnimClips.clear();
		mbMixamo = false;

		if (mScene)
		{
			mScene->Destroy();
			mScene = nullptr;
		}
		if (mManager)
		{
			mManager->Destroy();
			mManager = nullptr;
		}
	}

	eResult FBXLoader::LoadFbx(const std::fs::path& _strPath, bool _bStatic)
	{
		Reset();

		if (false == std::fs::exists(_strPath))
		{
			return eResult::Fail_PathNotExist;
		}
		
		mManager = FbxManager::Create();
		if (nullptr == mManager)
		{
			assert(nullptr);
			Reset();
			return eResult::Fail_Nullptr;
		}
		
		mManager->SetIOSettings(FbxIOSettings::Create(mManager, IOSROOT));

		mScene = FbxScene::Create(mManager, "");
		if (nullptr == mScene)
		{
			assert(nullptr);
			Reset();
			return eResult::Fail_Nullptr;
		}

		fbxsdk::FbxImporter* importer = FbxImporter::Create(mManager, "");

		if (false == importer->Initialize(_strPath.string().c_str(), -1, mManager->GetIOSettings()))
		{
			ERROR_MESSAGE_W(L"FBX 로더 초기화 실패");
			Reset();
			if (importer)
			{
				importer->Destroy();
			}
			return eResult::Fail_OpenFile;
		}

		importer->Import(mScene);


		//축 정보 변경
		//이전 코드
		//FbxAxisSystem originAxis = FbxAxisSystem::eMax;
		//originAxis = mScene->GetGlobalSettings().GetAxisSystem();
		//FbxAxisSystem DesireAxis = FbxAxisSystem::DirectX;
		//DesireAxis.ConvertScene(mScene);
		//originAxis = mScene->GetGlobalSettings().GetAxisSystem();

		//새로운 코드
		if (mScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
			mScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

		//전체 노드 갯수를 구해서 vector 크기를 예약 시켜놓는다.(이게 최대 수치임)
		int childCount = GetAllNodesCountRecursive(mScene->GetRootNode()) + 1;
		mContainers.reserve(childCount);


		//static 메쉬로 불러오기 설정이 아닐 경우
		if (false == _bStatic)
		{
			//애니메이션 이름을 쭉 받아온다.
			mScene->FillAnimStackNameArray(mAnimNames);

			if (mAnimNames.GetCount() > 0)
			{
				LoadAnimationClip();

				// 본 정보를 읽어온다.
				mBones.reserve(childCount);
				LoadSkeleton(mScene->GetRootNode());
				

				// 클립이 가지고 있는 프레임을 본 개수만큼 resize 해준다.
				// 원래 Animation Clip에서 하던것이다.
				for (size_t i = 0; i < mAnimClips.size(); ++i)
				{
					mAnimClips[i].vecBoneKeyFrame.resize(mBones.size());
				}
			}
		}

		// 삼각화(Triangulate)
		Triangulate(mScene->GetRootNode());

		// 메쉬 데이터 얻기
		LoadMeshContainer(mScene->GetRootNode());

		importer->Destroy();
		importer = nullptr;

		// 필요한 텍스쳐 로드
		LoadTexture();

		// 필요한 메테리얼 생성
		CreateMaterial(_strPath);

		return eResult::Success;
	}

	MATRIX FBXLoader::GetMatrixFromFbxMatrix(fbxsdk::FbxAMatrix& _mat)
	{
		MATRIX mat;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat.m[i][j] = (float)_mat.Get(i, j);
			}
		}
		return mat;
	}



	void FBXLoader::LoadMeshContainer(FbxNode* _pNode, bool _bStatic)
	{
		// 노드의 메쉬정보 읽기
		FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

		if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			//FbxAMatrix matGlobal = _pNode->EvaluateGlobalTransform();
			//matGlobal.GetR();
			FbxMesh* mesh = _pNode->GetMesh();
			if (mesh)
				LoadMesh(mesh, _bStatic);
		}

		// 해당 노드의 재질정보 읽기
		UINT iMtrlCnt = _pNode->GetMaterialCount();
		if (iMtrlCnt > 0)
		{
			for (UINT i = 0; i < iMtrlCnt; ++i)
			{
				FbxSurfaceMaterial* pMtrlSur = _pNode->GetMaterial(i);
				LoadMaterial(pMtrlSur);
			}
		}

		// 자식 노드 정보 읽기
		int iChildCnt = _pNode->GetChildCount();
		for (int i = 0; i < iChildCnt; ++i)
		{
			LoadMeshContainer(_pNode->GetChild(i));
		}
	}

	void FBXLoader::LoadMesh(FbxMesh* _pFBXMesh, bool _bStatic)
	{
		mContainers.push_back(tFBXContainer{});
		tFBXContainer& Container = mContainers.back();

		Container.Name = _pFBXMesh->GetName();

		// ControlPoint 는 위치정보를 담고 있는 배열이다.
		// 이 배열의 개수는 곧 정점의 개수가 된다.
		// 정점에 대응되는 요소들을 Resize 해준다
		int iVtxCnt = _pFBXMesh->GetControlPointsCount();
		Container.ResizeVertices(iVtxCnt);

		// 내부적으로 FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 
		// 시작주소를 얻어온다.
		FbxVector4* pFbxPos = _pFBXMesh->GetControlPoints();
		for (int i = 0; i < iVtxCnt; ++i)
		{
			// y와 z축이 바뀌어 있으므로 변경해준다.
			Container.vecPosition[i].x = (float)pFbxPos[i].mData[0];
			Container.vecPosition[i].y = (float)pFbxPos[i].mData[2];
			Container.vecPosition[i].z = (float)pFbxPos[i].mData[1];
		}

		// 폴리곤 개수
		int polygonCount = _pFBXMesh->GetPolygonCount();

		// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
		int materialCount = _pFBXMesh->GetNode()->GetMaterialCount();
		if (0 == materialCount)
		{
			materialCount = 1;
		}
		Container.vecIndexBuffers.resize(materialCount);

		// 정점 정보가 속한 subset 을 알기위해서 재질 정보를 얻어온다.
		FbxGeometryElementMaterial* material = _pFBXMesh->GetElementMaterial();

		//몇번째 인덱스인지
		int idxID = 0;

		//삼각형 수만큼 반복한다.
		for (int i = 0; i < polygonCount; ++i)
		{
			//이 폴리곤을 구성하는 정점의 수를 얻어온다.
			//삼각화를 진행한 상태이므로 3개가 들어오게 될것이다.
			int polygonSize = _pFBXMesh->GetPolygonSize(i);
			MH_ASSERT(3 == polygonSize);

			int idx[3] = {};
			for (int j = 0; j < polygonSize; ++j)
			{
				//현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를 구한다.(->해당 정점이 실제 몇번째 인덱스인지)
				int controlIndex = _pFBXMesh->GetPolygonVertex(i, j);
				idx[j] = controlIndex;

				GetTangent(_pFBXMesh, &Container, idxID, controlIndex);
				GetBinormal(_pFBXMesh, &Container, idxID, controlIndex);
				GetNormal(_pFBXMesh, &Container, idxID, controlIndex);

				GetUV(_pFBXMesh, &Container, ,)

			}
		}


		// 폴리곤을 구성하는 정점 개수
		int iPolySize = _pFBXMesh->GetPolygonSize(0);
		if (3 != iPolySize)
			assert(NULL); // Polygon 구성 정점이 3개가 아닌 경우

		UINT arrIdx[3] = {};
		UINT iVtxOrder = 0; // 폴리곤 순서로 접근하는 순번

		for (int i = 0; i < polygonCount; ++i)
		{
			for (int j = 0; j < iPolySize; ++j)
			{
				// i 번째 폴리곤에, j 번째 정점
				int iIdx = _pFBXMesh->GetPolygonVertex(i, j);
				arrIdx[j] = iIdx;

				//노말 정보를 받아오기
				GetTangent(_pFBXMesh, &Container, iIdx, iVtxOrder);
				GetBinormal(_pFBXMesh, &Container, iIdx, iVtxOrder);
				GetNormal(_pFBXMesh, &Container, iIdx, iVtxOrder);

				GetUV(_pFBXMesh, &Container, iIdx, _pFBXMesh->GetTextureUVIndex(i, j));

				++iVtxOrder;
			}
			UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
		}

		LoadAnimationData(_pFBXMesh, &Container);
	}

	void FBXLoader::LoadMaterial(FbxSurfaceMaterial* _pMtrlSur)
	{
		tFBXMaterial tMtrlInfo{};

		std::string str = _pMtrlSur->GetName();
		
		tMtrlInfo.strMtrlName = str;

		// Diff
		tMtrlInfo.tMtrl.DiffuseColor = GetMtrlData(_pMtrlSur
			, FbxSurfaceMaterial::sDiffuse
			, FbxSurfaceMaterial::sDiffuseFactor);

		// Amb
		tMtrlInfo.tMtrl.AmbientColor = GetMtrlData(_pMtrlSur
			, FbxSurfaceMaterial::sAmbient
			, FbxSurfaceMaterial::sAmbientFactor);

		// Spec
		tMtrlInfo.tMtrl.SpecularColor = GetMtrlData(_pMtrlSur
			, FbxSurfaceMaterial::sSpecular
			, FbxSurfaceMaterial::sSpecularFactor);

		// Emisv
		tMtrlInfo.tMtrl.EmissiveColor = GetMtrlData(_pMtrlSur
			, FbxSurfaceMaterial::sEmissive
			, FbxSurfaceMaterial::sEmissiveFactor);

		// Texture Name
		tMtrlInfo.strDiffuseTex = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sDiffuse);
		tMtrlInfo.strNormalTex = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sNormalMap);
		tMtrlInfo.strSpecularTex = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sSpecular);
		tMtrlInfo.strEmissiveTex = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sEmissive);


		mContainers.back().vecMtrl.push_back(tMtrlInfo);
	}

	void FBXLoader::GetTangent(FbxMesh* _pMesh
		, tFBXContainer* _pContainer
		, int _iIdx		 /*해당 정점의 인덱스*/
		, int _iVtxOrder /*폴리곤 단위로 접근하는 순서*/)
	{

		int iTangentCnt = _pMesh->GetElementTangentCount();
		//MH_ASSERT(1 == iTangentCnt);// 정점 1개가 포함하는 탄젠트 정보가 2개 이상이다.
		if (iTangentCnt < 1)
			return;

		// 탄젠트 data 의 시작 주소
		FbxGeometryElementTangent* pTangent = _pMesh->GetElementTangent();
		UINT iTangentIdx = 0;

		if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				iTangentIdx = _iVtxOrder;
			else
				iTangentIdx = pTangent->GetIndexArray().GetAt(_iVtxOrder);
		}
		else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				iTangentIdx = _iIdx;
			else
				iTangentIdx = pTangent->GetIndexArray().GetAt(_iIdx);
		}

		FbxVector4 vTangent = pTangent->GetDirectArray().GetAt(iTangentIdx);

		_pContainer->vecTangent[_iIdx].x = (float)vTangent.mData[0];
		_pContainer->vecTangent[_iIdx].y = (float)vTangent.mData[2];
		_pContainer->vecTangent[_iIdx].z = (float)vTangent.mData[1];
	}

	void FBXLoader::GetBinormal(FbxMesh* _pMesh, tFBXContainer* _pContainer, int _iIdx, int _iVtxOrder)
	{
		int iBinormalCnt = _pMesh->GetElementBinormalCount();
		//if (1 != iBinormalCnt)
		//	assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.
		if (iBinormalCnt < 1)
			return;

		// 종법선 data 의 시작 주소
		FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();
		UINT iBinormalIdx = 0;

		if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				iBinormalIdx = _iVtxOrder;
			else
				iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
		}
		else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				iBinormalIdx = _iIdx;
			else
				iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
		}

		FbxVector4 vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);

		_pContainer->vecBinormal[_iIdx].x = (float)vBinormal.mData[0];
		_pContainer->vecBinormal[_iIdx].y = (float)vBinormal.mData[2];
		_pContainer->vecBinormal[_iIdx].z = (float)vBinormal.mData[1];
	}

	void FBXLoader::GetNormal(FbxMesh* _pMesh, tFBXContainer* _pContainer, int _iIdx, int _iVtxOrder)
	{
		int iNormalCnt = _pMesh->GetElementNormalCount();

		//if (1 != iNormalCnt)
		//	assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.
		if (iNormalCnt < 1)
			return;

		// 종법선 data 의 시작 주소
		FbxGeometryElementNormal* pNormal = _pMesh->GetElementNormal();
		UINT iNormalIdx = 0;

		if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				iNormalIdx = _iVtxOrder;
			else
				iNormalIdx = pNormal->GetIndexArray().GetAt(_iVtxOrder);
		}
		else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				iNormalIdx = _iIdx;
			else
				iNormalIdx = pNormal->GetIndexArray().GetAt(_iIdx);
		}

		FbxVector4 vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);

		_pContainer->vecNormal[_iIdx].x = (float)vNormal.mData[0];
		_pContainer->vecNormal[_iIdx].y = (float)vNormal.mData[2];
		_pContainer->vecNormal[_iIdx].z = (float)vNormal.mData[1];
	}

	void FBXLoader::GetUV(FbxMesh* _pMesh, tFBXContainer* _pContainer, int _iIdx, int _iUVIndex)
	{
		FbxGeometryElementUV* pUV = _pMesh->GetElementUV();

		UINT iUVIdx = 0;
		if (pUV->GetReferenceMode() == FbxGeometryElement::eDirect)
			iUVIdx = _iIdx;
		else
			iUVIdx = pUV->GetIndexArray().GetAt(_iIdx);

		iUVIdx = _iUVIndex;
		FbxVector2 vUV = pUV->GetDirectArray().GetAt(iUVIdx);
		_pContainer->vecUV[_iIdx].x = (float)vUV.mData[0];
		_pContainer->vecUV[_iIdx].y = 1.f - (float)vUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
	}

	float4 FBXLoader::GetMtrlData(FbxSurfaceMaterial* _pSurface
		, const char* _pMtrlName
		, const char* _pMtrlFactorName)
	{
		FbxDouble3  vMtrl;
		FbxDouble	dFactor = 0.;

		FbxProperty tMtrlProperty = _pSurface->FindProperty(_pMtrlName);
		FbxProperty tMtrlFactorProperty = _pSurface->FindProperty(_pMtrlFactorName);

		if (tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid())
		{
			vMtrl = tMtrlProperty.Get<FbxDouble3>();
			dFactor = tMtrlFactorProperty.Get<FbxDouble>();
		}

		float4 vRetVal = float4((float)vMtrl.mData[0] * (float)dFactor, (float)vMtrl.mData[1] * (float)dFactor, (float)vMtrl.mData[2] * (float)dFactor, (float)dFactor);
		return vRetVal;
	}

	std::string FBXLoader::GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty)
	{
		std::string retStr;

		FbxProperty TextureProperty = _pSurface->FindProperty(_pMtrlProperty);
		if (TextureProperty.IsValid())
		{
			UINT iCnt = TextureProperty.GetSrcObjectCount();

			if (1 <= iCnt)
			{
				FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<FbxFileTexture>(0);
				if (nullptr != pFbxTex)
				{
					std::fs::path fullPath = pFbxTex->GetFileName();

					//절대 주소가 있을 경우 상대 주소를 만들어서 저장
					if (std::fs::exists(fullPath))
					{
						static std::fs::path resPath = PathMgr::GetContentPathAbsolute(eResourceType::MeshData);

						fullPath = fullPath.lexically_relative(resPath);

						retStr = fullPath.string();
					}
				}
			}
		}

		return retStr;
	}

	void FBXLoader::LoadTexture()
	{
		//일단 텍스처를 Texture 컨텐츠 폴더로 옮겨준다.
		const std::fs::path& fbxPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		const std::fs::path& texPath = PathMgr::GetContentPathRelative(eResourceType::Texture);

		//텍스처 폴더 발견 시 최초 한번 Texture 폴더로 이동시키기 위한 코드
		std::fs::path srcPathToDelete{};
		auto CopyAndLoadTex = 
			[&](std::string& _TextureRelativePath)->void
			{
				//비어있을경우 return
				if (_TextureRelativePath.empty())
					return;

				//이동 원본 경로와 목표 경로를 만들어준다.
				std::fs::path srcPath = fbxPath / _TextureRelativePath;
				std::fs::path destPath = texPath / _TextureRelativePath;

				//src에 파일이 있는데 dest에 없을 경우 복사
				if (std::fs::exists(srcPath) && false == std::fs::exists(destPath))
				{
					std::fs::path destDir = destPath;
					destDir.remove_filename();
					if (false == std::fs::exists(destDir))
					{
						std::fs::create_directories(destDir);
					}

					//파일을 복사하고 
					std::fs::copy(srcPath, destPath);
				}

				//바로 Texture Load. 로드 실패 시 false 반환
				if (nullptr == ResMgr::Load<Texture>(_TextureRelativePath))
				{
					_TextureRelativePath.clear();
					return;
				}
				else if (srcPathToDelete.empty())
				{
					srcPathToDelete = srcPath;
					srcPathToDelete.remove_filename();
				}
			};

		//순회를 돌며 텍스처 확인 후 이동시키고 로드까지 완료시킨다.
		for (UINT i = 0; i < mContainers.size(); ++i)
		{
			for (UINT j = 0; j < mContainers[i].vecMtrl.size(); ++j)
			{
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strDiffuseTex);
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strNormalTex);
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strSpecularTex);
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strEmissiveTex);
			}
		}
		
		//순회 다돌았는데 원본 폴더 경로 있으면 제거
		if (std::fs::exists(srcPathToDelete))
		{
			std::fs::remove_all(srcPathToDelete);
		}
	}

	void FBXLoader::CreateMaterial(const std::filesystem::path& _strPath)
	{
		//std::string strMtrlKey;
		//std::string strPath;

		for (UINT i = 0; i < mContainers.size(); ++i)
		{
			for (UINT j = 0; j < mContainers[i].vecMtrl.size(); ++j)
			{
				std::fs::path strMtrlKey = mContainers[i].vecMtrl[j].strMtrlName;
				if (strMtrlKey.empty())
				{
					//파일 이름에서 확장자 제거하고 이름만 받아옴
					strMtrlKey = _strPath.stem();

					//번호 붙여줌
					strMtrlKey += "_Mtrl";
					strMtrlKey += std::to_string(j);
				}
				
				//.json 확장자 붙여 줌
				strMtrlKey.replace_extension(strKey::Ext_Material);

				//strPath = "material\\";
				//strPath += strMtrlKey + ".mtrl";

				// 재질 이름
				mContainers[i].vecMtrl[j].strMtrlName = strMtrlKey.string();

				//std::string strName = strPath;

				// 이미 로딩된 재질이면 로딩된 것을 사용
				std::shared_ptr<Material> pMaterial = ResMgr::Find<Material>(mContainers[i].vecMtrl[j].strMtrlName);
				if (nullptr != pMaterial)
					continue;

				pMaterial = std::make_shared<Material>();

				// 상대경로가 곧 키
				pMaterial->SetKey(mContainers[i].vecMtrl[j].strMtrlName);


				//일단 기본 설정은 Deffered Shader 적용하는 걸로. 나중에 바꿀 것
				pMaterial->SetRenderingMode(eRenderingMode::DefferdOpaque);
				pMaterial->SetShader(ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::DefferedShader));

				
				{
					std::shared_ptr<Texture> pTex = ResMgr::Load<Texture>(mContainers[i].vecMtrl[j].strDiffuseTex);
					if (nullptr != pTex)
					{
						pMaterial->SetTexture(eTextureSlot::Albedo, pTex);
					}
				}

					
				{
					std::shared_ptr<Texture> pTex = ResMgr::Load<Texture>(mContainers[i].vecMtrl[j].strNormalTex);
					if (nullptr != pTex)
					{
						pMaterial->SetTexture(eTextureSlot::Normal, pTex);
					}
				}

				{
					std::shared_ptr<Texture> pTex = ResMgr::Load<Texture>(mContainers[i].vecMtrl[j].strSpecularTex);
					if (nullptr != pTex)
					{
						pMaterial->SetTexture(eTextureSlot::Specular, pTex);
					}
				}

				{
					std::shared_ptr<Texture> pTex = ResMgr::Find<Texture>(mContainers[i].vecMtrl[j].strEmissiveTex);
					if (nullptr != pTex)
					{
						pMaterial->SetTexture(eTextureSlot::Emissive, pTex);
					}
				}



				pMaterial->SetMaterialCoefficient(
					mContainers[i].vecMtrl[j].tMtrl.DiffuseColor
					, mContainers[i].vecMtrl[j].tMtrl.SpecularColor
					, mContainers[i].vecMtrl[j].tMtrl.AmbientColor
					, mContainers[i].vecMtrl[j].tMtrl.EmissiveColor);

				eResult result = pMaterial->Save(strMtrlKey);

				if (eResultFail(result))
				{
					ERROR_MESSAGE_W(L"FBX 변환 에러: Material 저장 실패");
				}

				ResMgr::Insert(pMaterial->GetKey(), pMaterial);
				
			}
		}
	}

	void FBXLoader::LoadSkeleton(FbxNode* _pNode)
	{
		mBones.reserve(200);


		//Root 노드는 Skeletal Node가 될 수 없다
		//https://download.autodesk.com/us/fbx/20102/fbx_sdk_help/index.html?url=WS8e4c2438b09b7f9c-5fe157071197ccd9b09-7ffe.htm,topicNumber=d0e4048
		int childCount = _pNode->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			LoaeBoneRecursive(_pNode->GetChild(i), 0, 0, -1);
		}
	}

	void FBXLoader::LoaeBoneRecursive(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx)
	{
		FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

		if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			mBones.push_back(tFBXBone{});
			tFBXBone& bone = mBones.back();

			bone.strBoneName = _pNode->GetName();
			if (mbMixamo)
			{
				bone.strBoneName.erase(0, 10);
			}

			bone.iDepth = _iDepth;
			bone.iParentIndx = _iParentIdx;
		}

		int iChildCount = _pNode->GetChildCount();
		for (int i = 0; i < iChildCount; ++i)
		{
			LoaeBoneRecursive(_pNode->GetChild(i), _iDepth + 1, (int)mBones.size(), _iIdx);
		}
	}

	void FBXLoader::LoadAnimationClip()
	{
		int iAnimCount = mAnimNames.GetCount();
		mAnimClips.reserve(iAnimCount);

		FbxTime::EMode timeMode = mScene->GetGlobalSettings().GetTimeMode();

		for (int i = 0; i < iAnimCount; ++i)
		{
			FbxAnimStack* pAnimStack = mScene->FindMember<FbxAnimStack>(mAnimNames[i]->Buffer());

			if (nullptr == pAnimStack)
				continue;

			//FbxAnimEvaluator* pevaluator = mScene->GetAnimationEvaluator();
			//mScene->SetCurrentAnimationStack();

			mAnimClips.push_back(tFBXAnimClip{});
			tFBXAnimClip& clip = mAnimClips.back();

			clip.strName = pAnimStack->GetName();
			
			//믹사모인지 확인
			if ("mixamo.com" == clip.strName)
			{
				mbMixamo = true;
			}

			FbxTakeInfo* take = mScene->GetTakeInfo(clip.strName.c_str());
			clip.StartTime = take->mLocalTimeSpan.GetStart();
			clip.EndTime = take->mLocalTimeSpan.GetStop();
			
			// GetFrameCount 함수를 호출하고  time모드를 넣어주면 시간을 프레임으로
			// 변환해준다. 몇프레임 짜리 애니메이션 인지를 구해준다.
			clip.TimeLength = clip.EndTime.GetFrameCount(timeMode) - clip.StartTime.GetFrameCount(timeMode);
			clip.TimeMode = timeMode;
		}
	}

	void FBXLoader::Triangulate(FbxNode* _pNode)
	{
		FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

		if (pAttr &&
			(pAttr->GetAttributeType() == FbxNodeAttribute::eMesh
				|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs
				|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
		{
			FbxGeometryConverter converter(mManager);
			converter.Triangulate(pAttr, true);
		}

		int iChildCount = _pNode->GetChildCount();

		for (int i = 0; i < iChildCount; ++i)
		{
			Triangulate(_pNode->GetChild(i));
		}
	}

	void FBXLoader::LoadAnimationData(FbxMesh* _pMesh, tFBXContainer* _pContainer)
	{
		// Animation Data 로드할 필요가 없음
		int iSkinCount = _pMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (iSkinCount <= 0 || mAnimClips.empty())
			return;

		_pContainer->bAnimation = true;

		// Skin 개수만큼 반복을하며 읽는다.	
		for (int i = 0; i < iSkinCount; ++i)
		{
			FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

			if (pSkin)
			{
				FbxSkin::EType eType = pSkin->GetSkinningType();
				if (FbxSkin::eRigid == eType || FbxSkin::eLinear)
				{
					// Cluster 를 얻어온다
					// Cluster == Joint == 관절
					int iClusterCount = pSkin->GetClusterCount();

					for (int j = 0; j < iClusterCount; ++j)
					{
						FbxCluster* pCluster = pSkin->GetCluster(j);

						if (!pCluster->GetLink())
							continue;

						// 현재 본 인덱스를 얻어온다.
						int iBoneIdx = FindBoneIndex(pCluster->GetLink()->GetName());
						if (-1 == iBoneIdx)
							assert(NULL);

						FbxAMatrix matNodeTransform = GetTransform(_pMesh->GetNode());

						// Weights And Indices 정보를 읽는다.
						LoadWeightsAndIndices(pCluster, iBoneIdx, _pContainer);

						// Bone 의 OffSet 행렬 구한다.
						LoadOffsetMatrix(pCluster, matNodeTransform, iBoneIdx, _pContainer);

						// Bone KeyFrame 별 행렬을 구한다.
						LoadKeyframeTransform(_pMesh->GetNode(), pCluster, matNodeTransform, iBoneIdx, _pContainer);
					}
				}
			}
		}
		CheckWeightAndIndices(_pMesh, _pContainer);
	}


	void FBXLoader::CheckWeightAndIndices(FbxMesh* _pMesh, tFBXContainer* _pContainer)
	{
		std::vector<std::vector<tFBXWeight>>::iterator iter = _pContainer->vecWI.begin();

		int iVtxIdx = 0;
		for (iter; iter != _pContainer->vecWI.end(); ++iter, ++iVtxIdx)
		{
			if ((*iter).size() > 1)
			{
				// 가중치 값 순으로 내림차순 정렬
				sort((*iter).begin(), (*iter).end()
					, [](const tFBXWeight& left, const tFBXWeight& right)
					{
						return left.dWeight > right.dWeight;
					}
				);

				double dWeight = 0.f;
				for (UINT i = 0; i < (*iter).size(); ++i)
				{
					dWeight += (*iter)[i].dWeight;
				}

				// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
				double revision = 0.f;
				if (dWeight > 1.0)
				{
					revision = 1.0 - dWeight;
					(*iter)[0].dWeight += revision;
				}

				if ((*iter).size() >= 4)
				{
					(*iter).erase((*iter).begin() + 4, (*iter).end());
				}
			}

			// 정점 정보로 변환, 
			float fWeights[4] = {};
			float fIndices[4] = {};

			for (UINT i = 0; i < (*iter).size(); ++i)
			{
				fWeights[i] = (float)(*iter)[i].dWeight;
				fIndices[i] = (float)(*iter)[i].iBoneIdx;
			}

			memcpy(&_pContainer->vecWeights[iVtxIdx], fWeights, sizeof(float4));
			memcpy(&_pContainer->vecIndices[iVtxIdx], fIndices, sizeof(float4));
		}
	}

	int FBXLoader::GetAllNodesCountRecursive(fbxsdk::FbxNode* _pNode)
	{
		int size = _pNode->GetChildCount();

		int retInt = size;
		for (int i = 0; i < size; ++i)
		{
			retInt += GetAllNodesCountRecursive(_pNode->GetChild(i));
		}

		return retInt;
	}



	void FBXLoader::LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster
		, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tFBXContainer* _pContainer)
	{
		if (mAnimClips.empty())
			return;

		FbxVector4	v1 = { 1, 0, 0, 0 };
		FbxVector4	v2 = { 0, 0, 1, 0 };
		FbxVector4	v3 = { 0, 1, 0, 0 };
		FbxVector4	v4 = { 0, 0, 0, 1 };
		FbxAMatrix	matReflect;
		matReflect.mData[0] = v1;
		matReflect.mData[1] = v2;
		matReflect.mData[2] = v3;
		matReflect.mData[3] = v4;

		mBones[_iBoneIdx]->matBone = _matNodeTransform;

		FbxTime::EMode eTimeMode = mScene->GetGlobalSettings().GetTimeMode();

		FbxLongLong llStartFrame = mAnimClips[0]->tStartTime.GetFrameCount(eTimeMode);
		FbxLongLong llEndFrame = mAnimClips[0]->tEndTime.GetFrameCount(eTimeMode);

		for (FbxLongLong i = llStartFrame; i < llEndFrame; ++i)
		{
			tFBXKeyFrame tFrame = {};
			FbxTime   tTime = 0;

			tTime.SetFrame(i, eTimeMode);

			FbxAMatrix matFromNode = _pNode->EvaluateGlobalTransform(tTime) * _matNodeTransform;
			FbxAMatrix matCurTrans = matFromNode.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(tTime);
			matCurTrans = matReflect * matCurTrans * matReflect;

			tFrame.dTime = tTime.GetSecondDouble();
			tFrame.matTransform = matCurTrans;

			mBones[_iBoneIdx]->vecKeyFrame.push_back(tFrame);
		}
	}

	void FBXLoader::LoadOffsetMatrix(FbxCluster* _pCluster
		, const FbxAMatrix& _matNodeTransform
		, int _iBoneIdx, tFBXContainer* _pContainer)
	{
		FbxAMatrix matClusterTrans;
		FbxAMatrix matClusterLinkTrans;

		_pCluster->GetTransformMatrix(matClusterTrans);
		_pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

		// Reflect Matrix
		FbxVector4 V0 = { 1, 0, 0, 0 };
		FbxVector4 V1 = { 0, 0, 1, 0 };
		FbxVector4 V2 = { 0, 1, 0, 0 };
		FbxVector4 V3 = { 0, 0, 0, 1 };

		FbxAMatrix matReflect;
		matReflect[0] = V0;
		matReflect[1] = V1;
		matReflect[2] = V2;
		matReflect[3] = V3;

		FbxAMatrix matOffset;
		matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * _matNodeTransform;
		matOffset = matReflect * matOffset * matReflect;

		mBones[_iBoneIdx]->matOffset = matOffset;
	}


	void FBXLoader::LoadWeightsAndIndices(FbxCluster* _pCluster
		, int _iBoneIdx
		, tFBXContainer* _pContainer)
	{
		int iIndicesCount = _pCluster->GetControlPointIndicesCount();

		for (int i = 0; i < iIndicesCount; ++i)
		{
			tFBXWeight tWI = {};

			// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
			tWI.iBoneIdx = _iBoneIdx;
			tWI.dWeight = _pCluster->GetControlPointWeights()[i];

			int iVtxIdx = _pCluster->GetControlPointIndices()[i];

			_pContainer->vecWI[iVtxIdx].push_back(tWI);
		}
	}



	int FBXLoader::FindBoneIndex(const std::string& _strBoneName)
	{
		for (UINT i = 0; i < mBones.size(); ++i)
		{
			if (mBones[i]->strBoneName == _strBoneName)
				return i;
		}

		return -1;
	}

	FbxAMatrix FBXLoader::GetTransform(FbxNode* _pNode)
	{
		const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(vT, vR, vS);
	}

}
