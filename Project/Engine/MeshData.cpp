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

#include <regex>

namespace mh
{
	MeshData::MeshData()
		: IRes(define::eResourceType::MeshData)
	{
	}

	MeshData::~MeshData()
	{
	}

	eResult MeshData::Save(const std::fs::path& _filePath)
	{
		eResult result = eResult::Fail;

		//MeshData는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path fileName = _filePath;
		fileName.replace_extension();
		fileName /= fileName;		
		fileName.replace_extension(strKey::Ext_MeshData);

		const std::fs::path& basePath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		IRes::Save(fileName);

		std::fs::path fullPath = PathMgr::CreateFullPathToContent(fileName, GetResType());

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

	eResult MeshData::Load(const std::fs::path& _filePath)
	{
		eResult result = eResult::Fail;

		//MeshData는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path fileName = _filePath;
		fileName.replace_extension();
		fileName /= fileName;
		fileName.replace_extension(strKey::Ext_MeshData);

		const std::fs::path& basePath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		IRes::Load(fileName);

		std::fs::path fullPath = PathMgr::CreateFullPathToContent(fileName, GetResType());

		Json::Value jVal;
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


		//Skeleton
		if (mSkeleton)
		{
			result = mSkeleton->Save(mSkeleton->GetKey());
			if (eResultFail(result))
				return result;
		}
		Json::MH::SavePtrStrKey(_pJson, JSON_KEY_PAIR(mSkeleton));



		//순회 돌아주면서 array 형태의 json에 append 해준다
		Json::Value& arrMeshCont = (*_pJson)[JSON_KEY(mMeshContainers)];
		for (size_t i = 0; i < mMeshContainers.size(); ++i)
		{
			Json::Value meshCont{};
			if (nullptr == mMeshContainers[i].pMesh || mMeshContainers[i].pMaterials.empty())
			{
				return eResult::Fail_InValid;
			}
			//Mesh
			result = mMeshContainers[i].pMesh->Save(mMeshContainers[i].pMesh->GetKey());
			if (eResultFail(result))
			{
				return result;
			}
			Json::MH::SavePtrStrKey(&meshCont, JSON_KEY(pMesh), mMeshContainers[i].pMesh);

			Json::Value& arrMtrl = meshCont[JSON_KEY(pMaterials)];
			//Material 저장
			for (size_t j = 0; j < mMeshContainers[i].pMaterials.size(); ++j)
			{
				result = mMeshContainers[i].pMaterials[j]->Save(mMeshContainers[i].pMaterials[j]->GetKey());
				if (eResultFail(result))
				{
					return result;
				}

				if (nullptr == mMeshContainers[i].pMaterials[j])
				{
					return eResult::Fail_Nullptr;
				}
				else
				{
					arrMtrl.append(mMeshContainers[i].pMaterials[j]->GetKey());
				}

			}

			arrMeshCont.append(meshCont);
		}

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

			//Mesh
			std::string meshStrKey = Json::MH::LoadPtrStrKey(&(*iter), JSON_KEY(pMesh), cont.pMesh);
			cont.pMesh = ResMgr::Load<Mesh>(meshStrKey);
			if (nullptr == cont.pMesh)
				return eResult::Fail_Empty;

			//Materials
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

	eResult MeshData::ConvertFBX(
		const std::fs::path& _fbxPath, bool _bStatic,
		const std::fs::path& _dirAndFileName
	)
	{
		eResult result = LoadFromFBX(_fbxPath, _bStatic, _dirAndFileName);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"FBX로부터 로드 실패.");
			return result;
		}



		//다른게 다 진행됐으면 저장 진행
		//키값 만들고 세팅하고
		result = Save(_dirAndFileName);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"MeshData 저장에 실패했습니다.");
			return result;
		}

		return eResult::Success;
	}

	eResult MeshData::LoadFromFBX(const std::fs::path& _fbxPath, bool _bStatic, const std::fs::path& _dirAndFileName)
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

		//Res/MeshData/Player
		std::fs::path filePath = _dirAndFileName;
		filePath.replace_extension();
		//Res/MeshData/Player/Player
		filePath /= filePath;

		//Bone 정보 로드
		mSkeleton = std::make_shared<Skeleton>();

		//Key 설정
		{
			std::fs::path skltStrKey = filePath;
			skltStrKey.replace_extension(strKey::Ext_Skeleton);
			mSkeleton->SetKey(skltStrKey.string());
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
				//비어있을 경우 이름을 만들어준다
				std::fs::path strKey{}; 
				if (containers[i].Name.empty())
				{
					strKey = filePath;
					strKey.replace_extension();
					strKey += "_";
					strKey += std::to_string(i);
				}
				else
				{
					strKey = _dirAndFileName;
					strKey /= containers[i].Name;
				}

				//.msh로 확장자를 변경
				strKey.replace_extension(define::strKey::Ext_Mesh);
				//Key로 Mesh를 저장
				meshCont.pMesh->SetKey(strKey.string());
			}

			// 메테리얼 가져오기
			for (UINT j = 0; j < containers[i].vecMtrl.size(); ++j)
			{

				std::shared_ptr<Material> mtrl = 
					ConvertMaterial(&(containers[i].vecMtrl[j]), _dirAndFileName);
				if (nullptr == mtrl)
				{
					ERROR_MESSAGE_W(L"머티리얼 로드에 실패했습니다.");
					return eResult::Fail_InValid;
				}

				meshCont.pMaterials.push_back(mtrl);
			}
			mMeshContainers.push_back(meshCont);
		}

		return eResult::Success;
	}

	eResult MeshData::AddAnimationFromFBX(const std::fs::path& _fbxPath, const std::fs::path& _meshDataName)
	{
		if (false == std::fs::exists(_fbxPath))
		{
			ERROR_MESSAGE_W(L"파일을 찾지 못했습니다.");
			return eResult::Fail_PathNotExist;
		}

		FBXLoader loader{};
		eResult result = loader.LoadFbx(_fbxPath, false);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"FBX 불러오기 실패.");
			return result;
		}

		std::unique_ptr<Skeleton> skeletonOfFBX = std::make_unique<Skeleton>();
		result = skeletonOfFBX->CreateFromFBX(&loader);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"FBX로부터 스켈레톤 로딩 실패.");
			return result;
		}

		//지금 필요한건 FBX에 저장된 Skeleton과 Animation 정보 뿐임
		std::unique_ptr<Skeleton> skeletonOfProj = std::make_unique<Skeleton>();

		//Skeleton의 실제 경로: Player/Player
		result = skeletonOfProj->Load(_meshDataName / _meshDataName);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"프로젝트 스켈레톤 불러오기 실패.");
			return result;
		}

		if (false == skeletonOfProj->CopyAnimationFromOther((*skeletonOfFBX), _meshDataName))
		{
			MessageBoxW(nullptr, L"스켈레톤 구조가 일치하지 않아 애니메이션을 추가할 수 없습니다.", nullptr, MB_OK);
			return eResult::Fail;
		}

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

		std::fs::path strKey = _texDestDir.filename();
		strKey /= _fbxMtrl->strMtrlName;
		strKey.replace_extension(strKey::Ext_Material);
		mtrl->SetKey(strKey.string());

		mtrl->SetMaterialCoefficient(_fbxMtrl->DiffuseColor, _fbxMtrl->SpecularColor, _fbxMtrl->AmbientColor, _fbxMtrl->EmissiveColor);

		std::fs::path texDir = PathMgr::GetContentPathRelative(eResourceType::Texture);
		texDir /= _texDestDir;

		//media directory(텍스처같은 파일들) 옮겨졌는지 여부 저장 변수
		bool bMediaDirMoved = false;

		//텍스처 옮기기 위한 람다 함수
		auto CopyAndLoadTex =
			[&](const std::string& _srcTexPath)->std::shared_ptr<Texture>
			{
				//비어있을경우 return
				if (_srcTexPath.empty())
					return nullptr;

				if (false == bMediaDirMoved)
				{
					std::fs::path srcTexPath = _srcTexPath;
					srcTexPath = srcTexPath.parent_path();

					if (false == std::fs::exists(texDir))
					{
						std::fs::create_directories(texDir);
					}

					if (std::fs::exists(srcTexPath))
					{
						const auto copyOption =
							std::fs::copy_options::overwrite_existing
							| std::fs::copy_options::recursive;

						std::fs::copy(srcTexPath, texDir, copyOption);
						std::fs::remove_all(srcTexPath);

						bMediaDirMoved = true;
					}
				}

				std::shared_ptr<Texture> newTex = std::make_shared<Texture>();

				std::fs::path texKey = _texDestDir.filename();
				texKey /= std::fs::path(_srcTexPath).filename();

				//바로 Texture Load. 로드 실패 시 false 반환
				if (eResultFail(newTex->Load(texKey)))
				{
					newTex = nullptr;
				}

				return newTex;
			};

		mtrl->SetTexture(eTextureSlot::Albedo, CopyAndLoadTex(_fbxMtrl->strDiffuseTex));
		mtrl->SetTexture(eTextureSlot::Normal, CopyAndLoadTex(_fbxMtrl->strNormalTex));
		mtrl->SetTexture(eTextureSlot::Specular, CopyAndLoadTex(_fbxMtrl->strSpecularTex));
		mtrl->SetTexture(eTextureSlot::Emissive, CopyAndLoadTex(_fbxMtrl->strEmissiveTex));

		std::shared_ptr<GraphicsShader> defferedShader = ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::DefferedShader);
		mtrl->SetShader(defferedShader);

		mtrl->SetRenderingMode(eRenderingMode::DefferdOpaque);

		CheckMHMaterial(mtrl, texDir);

		return mtrl;
	}

	void MeshData::CheckMHMaterial(std::shared_ptr<Material> _mtrl, const std::fs::path& _texDestDir)
	{
		if (nullptr == _mtrl)
			return;

		constexpr const char* texSuffix[] =
		{
			"_BML",
			"_NM",
			"_XM",
			"_EM",
			"_RMT"
		};
		constexpr size_t texSuffixSize = sizeof(texSuffix) / sizeof(const char*);

		std::string strSuffix = "(";
		for (size_t i = 0; i < texSuffixSize; ++i)
		{
			strSuffix += texSuffix[i];

			if (i + 1 < texSuffixSize)
			{
				strSuffix += "|";
			}
		}
		strSuffix += ")(\\..+)$";
		const std::regex regexPrefix("(.+)(DXT\\d_|BC\\d_)");
		const std::regex regexSuffix(strSuffix);


		//텍스처 기본 생성 파일명을 통해서 몬스터헌터 텍스처 파일인지 확인
		bool isMHTex = false;
		for (size_t i = 0; i < texSuffixSize; ++i)
		{
			std::shared_ptr<Texture> tex = _mtrl->GetTexture((eTextureSlot)i);
			if (tex)
			{
				std::string texKey = tex->GetKey();
				size_t pos = texKey.find(texSuffix[i]);
				if (std::string::npos != pos)
				{
					isMHTex = true;
				}
				else
				{
					//하나라도 이름이 일치하지 않으면 몬스터헌터 포맷이 아닌것으로 간주
					isMHTex = false;
					break;
				}
			}
		}
		

		//몬스터헌터 재질이 맞는것이 확인될 경우 못 가져온 텍스처 가져오는 작업 진행
		if (isMHTex)
		{
			for (size_t i = 0; i < texSuffixSize; ++i)
			{
				//i번째 텍스처가 있고
				std::shared_ptr<Texture> tex = _mtrl->GetTexture((eTextureSlot)i);
				if (tex)
				{
					for (size_t j = 0; j < texSuffixSize; ++j)
					{
						//j번째 텍스처는 없다면
						std::shared_ptr<Texture> newTex = _mtrl->GetTexture((eTextureSlot)j);
						if (nullptr == newTex)
						{
							//i번째 텍스처의 이름을 가져와서
							std::string texKey = tex->GetKey();

							//regex 돌려서 prefix suffix 제거하고
							texKey = std::regex_replace(texKey, regexPrefix, "");
							texKey = std::regex_replace(texKey, regexSuffix, "");

							//파일 순회를 돌아주면서
							for (const auto& dirIter : std::fs::directory_iterator(_texDestDir))
							{
								std::string fileName = dirIter.path().filename().string();
								
								//이름이 일치하는 파일명이 아니면 continue
								if (std::string::npos == fileName.find(texKey))
									continue;

								//이름이 일치할 경우 j번째 suffix와 일치하는지 확인
								if (std::string::npos != fileName.find(texSuffix[j]))
								{
									//일치할 경우 이 텍스처를 material에 추가
									newTex = std::make_shared<Texture>();

									std::fs::path texKey = _texDestDir.filename();
									texKey /= dirIter.path().filename();
									newTex->SetKey(texKey.string());

									newTex->Load(texKey);

									_mtrl->SetTexture((eTextureSlot)j, newTex);
								}
							}
						}
					}
				}
			}
		}
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

