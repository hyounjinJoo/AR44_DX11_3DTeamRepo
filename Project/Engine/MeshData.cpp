#include "PCH_Engine.h"
#include "MeshData.h"

#include "define_Util.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "FBXLoader.h"
#include "GameObject.h"
#include "Com_DummyTransform.h"
#include "Com_DummyAnimator.h"
#include "Com_Renderer_3DAnimMesh.h"
#include "Com_Animator3D.h"
#include "define_Util.h"

#include "PathMgr.h"
#include "json-cpp/json.h"
#include "Skeleton.h"
#include "object.h"

namespace mh
{
	MeshData::MeshData()
		: IRes(define::eResourceType::MeshData)
	{
	}

	MeshData::~MeshData()
	{
	}

	eResult MeshData::Save(const std::fs::path& _filePath, const std::fs::path& _basePath)
	{
		eResult result = eResult::Fail;

		//MeshData는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path fullPath = IRes::CreateFullPath(_filePath, _basePath);
		fullPath.replace_extension();
		//경로가 없을 시 경로를 생성
		if (false == std::fs::exists(fullPath))
		{
			std::fs::create_directories(fullPath);
		}

		//이 경로로 MeshContainer, Skeleton 저장
		if (mSkeleton)
		{
			result = mSkeleton->Save(mSkeleton->GetKey(), fullPath);
			if (eResultFail(result))
				return result;
		}
		
		for (size_t i = 0; i < mMeshContainers.size(); ++i)
		{
			//Mesh 저장
			result = mMeshContainers[i].pMesh->Save(mMeshContainers[i].pMesh->GetKey(), fullPath);
			if (eResultFail(result))
			{
				return result;
			}
			
			//Material 저장
			for (size_t j = 0; j < mMeshContainers[i].pMaterials.size(); ++j)
			{
				result = mMeshContainers[i].pMaterials[j]->Save(mMeshContainers[i].pMaterials[j]->GetKey(), fullPath);
				if (eResultFail(result))
				{
					return result;
				}
			}
		}

		//마지막으로 자신을 json 확장자로 변경하고 저장
		fullPath.replace_extension();
		fullPath /= _filePath;
		fullPath.replace_extension(strKey::Ext_MeshData);

		std::ofstream ofs(fullPath);
		if (false == ofs.is_open())
			return eResult::Fail_OpenFile;

		Json::Value jVal{};
		result = SaveJson(&jVal);
		if (eResultFail(result))
			return result;
		
		ofs << jVal;
		ofs.close();

		return eResult::Success;
	}

	eResult MeshData::Load(const std::fs::path& _filePath, const std::fs::path& _basePath)
	{
		eResult result = eResult::Fail;

		//MeshData는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path fullPath = IRes::CreateFullPath(_filePath, _basePath);
		fullPath.replace_extension();
		//경로가 없을 시 경로를 생성
		if (false == std::fs::exists(fullPath))
		{
			std::fs::create_directories(fullPath);
		}
		//파일명까지 생성
		fullPath /= _filePath;

		//이 경로로 MeshContainer, Skeleton 저장
		if (mSkeleton)
		{
			fullPath.replace_extension(strKey::Ext_Skeleton);
			result = mSkeleton->Load(_filePath, fullPath);
			if (eResultFail(result))
				return result;
		}


		//마지막으로 자기 자신 저장
		Json::Value jVal;
		fullPath.replace_extension();
		fullPath /= _filePath;
		fullPath.replace_extension(strKey::Ext_MeshData);
		std::ifstream ifs(fullPath);
		if (false == ifs.is_open())
		{
			return eResult::Fail_OpenFile;
		}

		ifs >> jVal;
		ifs.close();
		result = LoadJson(&jVal);
		if (eResultFail(result))
			return result;

		return eResult::Success;
	}

	//eResult MeshData::ConvertFBX(const std::fs::path& _fbxFullPath, bool _bStatic, const std::fs::path& _dirAndFileName)
	//{
	//	if (false == std::fs::exists(_fbxFullPath))
	//	{
	//		ERROR_MESSAGE_W(L"파일이 없습니다.");
	//		return eResult::Fail_OpenFile;
	//	}
	//	
	//	std::unique_ptr<MeshData> meshData = std::make_unique<MeshData>();
	//	eResult result = meshData->LoadFromFBX(_fbxFullPath, _bStatic);
	//	if (eResultFail(result))
	//		return result;

	//	meshData->Save(_dirAndFileName);

	//	return eResult::Success;
	//}

	eResult MeshData::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;
		eResult result = IRes::SaveJson(_pJson);
		if (eResultFail(result))
			return result;

		//비어있을 경우 저장 불가
		if (mMeshContainers.empty())
		{
			return eResult::Fail_InValid;
		}

		//순회 돌아주면서 array 형태의 json에 append 해준다
		Json::Value& jsonMeshCont = (*_pJson)[JSON_KEY(mMeshContainers)];
		for (size_t i = 0; i < mMeshContainers.size(); ++i)
		{
			Json::Value arrElement;
			if (nullptr == mMeshContainers[i].pMesh || mMeshContainers[i].pMaterials.empty())
			{
				return eResult::Fail_InValid;
			}

			Json::MH::SavePtrStrKey(&arrElement, JSON_KEY(pMesh), mMeshContainers[i].pMesh);
			Json::MH::SavePtrStrKeyVector(&arrElement, JSON_KEY(pMaterials), mMeshContainers[i].pMaterials);

			jsonMeshCont.append(arrElement);
		}

		Json::MH::SavePtrStrKey(_pJson, JSON_KEY_PAIR(mSkeleton));

		return eResult::Success;
	}


	eResult MeshData::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;
		eResult result = IRes::LoadJson(_pJson);
		if (eResultFail(result))
			return result;
		
		mMeshContainers.clear();

		//mesh container 순회 돌아주면서 하나씩 Load
		const Json::Value& jsonMeshCont = (*_pJson)[JSON_KEY(mMeshContainers)];
		for (Json::ValueConstIterator iter = jsonMeshCont.begin();
			iter != jsonMeshCont.end();
			++iter)
		{
			tMeshContainer cont{};

			//Mesh Load
			std::string meshStrKey = Json::MH::LoadPtrStrKey(&(*iter), JSON_KEY(pMesh), cont.pMesh);
			cont.pMesh = ResMgr::Load<Mesh>(meshStrKey);
			if (nullptr == cont.pMesh)
				return eResult::Fail_Empty;

			//Materials Load
			const auto& materialsStrKey = Json::MH::LoadPtrStrKeyVector(&(*iter), JSON_KEY(pMaterials), cont.pMaterials);
			for (size_t i = 0; i < materialsStrKey.size(); ++i)
			{
				std::shared_ptr<Material> mtrl = ResMgr::Load<Material>(materialsStrKey[i]);
				cont.pMaterials.push_back(mtrl);
			}

			mMeshContainers.push_back(cont);
		}

		const std::string& skeletonKey = Json::MH::LoadPtrStrKey(_pJson, JSON_KEY_PAIR(mSkeleton));
		if (false == skeletonKey.empty())
		{
			mSkeleton = std::make_shared<Skeleton>();
			result = mSkeleton->Load(skeletonKey);
			if (eResultFail(result))
				return result;
		}

		if (mSkeleton)
		{
			for (size_t i = 0; i < mMeshContainers.size(); ++i)
			{
				if (mMeshContainers[i].pMesh)
				{
					mMeshContainers[i].pMesh->SetSkeleton(mSkeleton);
				}
			}
		}

		return eResult::Success;
	}


	GameObject* MeshData::Instantiate()
	{
		if (mMeshContainers.empty())
		{
			return nullptr;
		}

		std::unique_ptr<GameObject> uniqObj = std::make_unique<GameObject>();
		Com_Transform* tr = uniqObj->AddComponent<Com_Transform>();

		//스켈레톤 있고 + 애니메이션 데이터가 있을 경우 Animator 생성
		Com_Animator3D* animator = nullptr;
		if (mSkeleton)
		{
			animator = uniqObj->AddComponent<Com_Animator3D>();
			animator->SetSkeleton(mSkeleton);
		}


		//사이즈가 딱 하나일 경우: GameObject 본체에 데이터를 생성
		if (1u == (UINT)mMeshContainers.size())
		{
			Com_Renderer_Mesh* renderer = nullptr;
			if (animator)
			{
				//수동으로 애니메이터를 설정
				auto* renderer3D = uniqObj->AddComponent<Com_Renderer_3DAnimMesh>();
				renderer = static_cast<Com_Renderer_Mesh*>(renderer3D);
			}
			else
			{
				renderer = uniqObj->AddComponent<Com_Renderer_Mesh>();
			}

			MH_ASSERT(renderer);
			SetRenderer(renderer, 0);
		}

		//여러 개의 container를 가지고 있을 경우: 하나의 부모 object에 여러개의 child를 생성해서 각각 Meshrenderer에 할당
		else
		{
			for (size_t i = 0; i < mMeshContainers.size(); ++i)
			{
				GameObject* child = uniqObj->AddChild(new GameObject);
				child->AddComponent<Com_DummyTransform>();
				child->AddComponent<Com_DummyAnimator>();

				//ComMgr로부터 Mesh 렌더러를 받아와서 MultiMesh에 넣어준다.
				Com_Renderer_Mesh* renderer = nullptr;
				if (animator)
				{
					//수동으로 애니메이터를 설정
					auto* renderer3D = child->AddComponent<Com_Renderer_3DAnimMesh>();
					renderer = static_cast<Com_Renderer_Mesh*>(renderer3D);
				}
				else
				{
					renderer = child->AddComponent<Com_Renderer_Mesh>();
				}
				
				MH_ASSERT(renderer);
				SetRenderer(renderer, (UINT)i);
			}
		}



		//다 됐을 경우 unique_ptr 관리 해제하고 주소 반환
		return uniqObj.release();
	}

	eResult MeshData::LoadAndConvertFBX(
		const std::fs::path& _fbxPath, bool _bStatic,
		const std::fs::path& _dirAndFileName
	)
	{
		if (false == std::fs::exists(_fbxPath))
		{
			ERROR_MESSAGE_W(L"파일을 찾지 못했습니다.");
			return eResult::Fail_PathNotExist;
		}
		else if (_dirAndFileName.empty())
		{
			ERROR_MESSAGE_W(L"파일명을 설정하지 않았습니다.");
			return eResult::Fail_PathNotExist;
		}

		FBXLoader loader{};
		eResult result = loader.LoadFbx(_fbxPath, _bStatic);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"FBX 불러오기 실패.");
			return result;
		}


		std::fs::path basePath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		basePath /= _dirAndFileName;
		if (false == std::fs::exists(basePath))
		{
			std::fs::create_directories(basePath);
		}


		//Bone 정보 로드
		mSkeleton = std::make_shared<Skeleton>();

		//Key 설정
		{
			std::fs::path skeletonKey = _dirAndFileName;
			skeletonKey.replace_extension(strKey::Ext_Skeleton);
			mSkeleton->SetKey(skeletonKey.string());
		}
		result = mSkeleton->CreateFromFBX(&loader);

		//애니메이션 정보가 없을 경우에는 도로 제거
		if (eResult::Fail_Empty == result)
		{
			mSkeleton = nullptr;
		}
		//그 외의 이유로 실패했을 경우에는 에러
		else if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"Skeleton 로드 실패.");
			return result;
		}


		const std::vector<tFBXContainer>& containers = loader.GetContainers();
		for (size_t i = 0; i < containers.size(); ++i)
		{
			tMeshContainer meshCont{};

			//가져올 메쉬를 생성
			meshCont.pMesh = std::make_shared<Mesh>();

			result = meshCont.pMesh->CreateFromContainer(&(containers[i]));
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"FBX로부터 메쉬 정보를 읽어오는 데 실패했습니다.");
				return eResult::Fail;
			}

			//스켈레톤 주소를 지정
			meshCont.pMesh->SetSkeleton(mSkeleton);

			if (nullptr != meshCont.pMesh)
			{
				//기본적으로는 컨테이너 이름을 사용
				std::fs::path strKey = containers[i].Name;

				//비어있을 경우 이름을 만들어준다
				if (strKey.empty())
				{
					strKey = _dirAndFileName;
					strKey.replace_extension();
					strKey += "_";
					strKey += std::to_string(i);
				}

				//.msh로 확장자를 변경
				strKey.replace_extension(define::strKey::Ext_Mesh);
				//Key로 Mesh를 저장
				meshCont.pMesh->SetKey(strKey.string());
			}


			// 메테리얼 가져오기
			for (UINT j = 0; j < containers[i].vecMtrl.size(); ++j)
			{

				std::shared_ptr<Material> mtrl = ConvertMaterial(&(containers[i].vecMtrl[j]), basePath);
				if(nullptr == mtrl)
				{
					ERROR_MESSAGE_W(L"머티리얼 로드에 실패했습니다.");
					return eResult::Fail_InValid;
				}

				meshCont.pMaterials.push_back(mtrl);
			}
			mMeshContainers.push_back(meshCont);
		}

		//다른게 다 진행됐으면 자신도 저장
		//키값 만들고 세팅하고
		std::fs::path strKeyMeshData = _dirAndFileName;
		strKeyMeshData.replace_extension(strKey::Ext_MeshData);
		SetKey(strKeyMeshData.string());
		result = Save(_dirAndFileName);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"MeshData 저장에 실패했습니다.");
			return result;
		}
		

		//모두 문제없이 처리되었을 경우 메쉬와 재질을 ResMgr에 전부 추가한다.
		for (size_t i = 0; i < mMeshContainers.size(); ++i)
		{
			//ResMgr::Insert(mMeshContainers[i].pMesh->GetKey(), mMeshContainers[i].pMesh);

			//재질은 FBX Loader에서 추가가 되어있는 상태라서 여기서 처리할 필요 없음.
			//for (size_t j = 0; j < mMeshContainers[j].pMaterials.size(); ++i)
			//{
			//	ResMgr::Insert(mMeshContainers[i].pMaterials[j]->GetKey(), mMeshContainers[i].pMaterials[j]);
			//}
		}

		//전부 저장에 성공했을 경우 ResMgr에서 이 주소(MeshData)를 리소스에 추가한다
		//애초에 호출한 클래스가 ResMgr임

		return eResult::Success;
	}

	std::shared_ptr<Material> MeshData::ConvertMaterial(const tFBXMaterial* _fbxMtrl, const std::fs::path& _texDestDir)
	{
		if (nullptr == _fbxMtrl || _texDestDir.empty())
		{
			return nullptr;
		}
		//material 하나 생성
		std::shared_ptr<Material> mtrl = std::make_shared<Material>();

		std::fs::path strKey = _fbxMtrl->strMtrlName;
		strKey.replace_extension(strKey::Ext_Material);
		mtrl->SetKey(strKey.string());

		mtrl->SetMaterialCoefficient(_fbxMtrl->DiffuseColor, _fbxMtrl->SpecularColor, _fbxMtrl->AmbientColor, _fbxMtrl->EmissiveColor);

		//텍스처 옮기기 위한 람다 함수
		auto CopyAndLoadTex =
			[&](const std::string& _srcTexPath)->std::shared_ptr<Texture>
			{
				//비어있을경우 return
				if (_srcTexPath.empty())
					return nullptr;
				//이동 원본 경로와 목표 경로를 만들어준다.

				std::fs::path srcTexPath = _srcTexPath;
				std::fs::path destTextPath = _texDestDir / srcTexPath.filename();
				//src에 파일이 있는데 dest에 없을 경우 복사
				if (std::fs::exists(srcTexPath) && false == std::fs::exists(destTextPath))
				{
					//폴더 예외 확인
					std::fs::path destDir = destTextPath.parent_path();
					if (false == std::fs::exists(destDir))
					{
						std::fs::create_directories(destDir);
					}

					//파일을 복사하고 기존 파일은 제거
					std::fs::copy(srcTexPath, _texDestDir);
					std::fs::remove(srcTexPath);
				}

				
				std::shared_ptr<Texture> newTex = std::make_shared<Texture>();
				//바로 Texture Load. 로드 실패 시 false 반환
				
				if (eResultFail(newTex->Load(srcTexPath.filename(), _texDestDir)))
				{
					newTex = nullptr;
				}

				return newTex;
			};

		mtrl->SetTexture(eTextureSlot::Albedo, CopyAndLoadTex(_fbxMtrl->strDiffuseTex));
		mtrl->SetTexture(eTextureSlot::Normal, CopyAndLoadTex(_fbxMtrl->strNormalTex));
		mtrl->SetTexture(eTextureSlot::Specular, CopyAndLoadTex(_fbxMtrl->strSpecularTex));
		mtrl->SetTexture(eTextureSlot::Emissive, CopyAndLoadTex(_fbxMtrl->strEmissiveTex));

		return mtrl;
	}

	bool MeshData::SetRenderer(Com_Renderer_Mesh* _renderer, UINT _idx)
	{
		if (nullptr == _renderer)
			return false;

		else if (_idx >= (UINT)mMeshContainers.size())
			return false;

		//Mesh 또는 Material은 없을 리가 없음(생성할 때 예외처리 함)
		_renderer->SetMesh(mMeshContainers[_idx].pMesh);

		for (size_t i = 0; i < mMeshContainers[_idx].pMaterials.size(); ++i)
		{
			_renderer->SetMaterial(mMeshContainers[_idx].pMaterials[i], (UINT)i);
		}

		return true;
	}
}

