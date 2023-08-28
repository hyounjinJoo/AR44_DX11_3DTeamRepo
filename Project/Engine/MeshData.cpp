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

	eResult MeshData::Save(const std::filesystem::path& _fileName)
	{
		std::fs::path fullPath = PathMgr::GetContentPathRelative(GetResType());
		if (false == std::fs::exists(fullPath))
			std::fs::create_directories(fullPath);

		fullPath /= _fileName;
		fullPath.replace_extension(strKey::Ext_MeshData);

		std::ofstream ofs(fullPath);
		if (false == ofs.is_open())
			return eResult::Fail_OpenFile;

		Json::Value jVal{};
		eResult result = SaveJson(&jVal);
		if (eResultFail(result))
			return result;
		
		ofs << jVal;
		ofs.close();

		return eResult::Success;
	}

	eResult MeshData::Load(const std::filesystem::path& _filePath)
	{
		std::string ext = StringConv::UpperCaseReturn(_filePath.extension().string());

		//FBX일 경우에는 FBXLoader를 통해서 가져온다.
		if (".FBX" == ext)
		{
			eResult result = LoadFromFBX(_filePath);
			if (eResultFail(result))
				return result;
		}

		//그렇지 않을 경우 json을 통해 로드
		else if (".JSON" == ext)
		{
			std::fs::path fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
			fullPath /= _filePath;
			if (false == std::fs::exists(fullPath.parent_path()))
			{
				if (false == std::fs::create_directories(fullPath))
				{
					ERROR_MESSAGE_W(L"경로 정보가 이상합니다.");
					return eResult::Fail_PathNotExist;
				}
			}

			Json::Value jVal;
			std::ifstream ifs(fullPath);
			if (false == ifs.is_open())
			{
				return eResult::Fail_OpenFile;
			}

			ifs >> jVal;
			ifs.close();
			eResult result = LoadJson(&jVal);
			if (eResultFail(result))
				return result;
		}

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
			mSkeleton = std::make_unique<Skeleton>();
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
					mMeshContainers[i].pMesh->SetSkeleton(mSkeleton.get());
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
		if (mSkeleton && mSkeleton->IsAnimMesh())
		{
			animator = uniqObj->AddComponent<Com_Animator3D>();
			animator->SetSkeleton(mSkeleton.get());
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

	eResult MeshData::LoadFromFBX(const std::filesystem::path& _fileName)
	{
		std::fs::path fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		fullPath /= _fileName;
		if (false == std::fs::exists(fullPath.parent_path()))
		{
			if (false == std::fs::create_directories(fullPath))
			{
				ERROR_MESSAGE_W(L"경로 정보가 이상합니다.");
				return eResult::Fail_PathNotExist;
			}
		}

		FBXLoader loader{};
		loader.Init();
		eResult result = loader.LoadFbx(fullPath);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"FBX 불러오기 실패.");
			return result;
		}

		//Bone 정보 및 애니메이션 로드
		mSkeleton = std::make_unique<Skeleton>();

		//Key 설정
		{
			std::fs::path strKey = _fileName;
			strKey.replace_extension(define::strKey::Ext_Skeleton);
			mSkeleton->SetKey(strKey.string());
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
		if(mSkeleton)
			mSkeleton->Save(_fileName);

		//컨테이너 갯수만큼 순회를 돌아준다.
		int contCount = loader.GetContainerCount();
		for (int i = 0; i < contCount; ++i)
		{
			tMeshContainer meshCont{};

			//컨테이너를 가져옴
			const tFBXContainer* cont = loader.GetContainer(i);

			//예외 처리
			if (nullptr == cont)
			{
				mMeshContainers.clear();
				std::wstring errorMsg = std::to_wstring(i);
				errorMsg += L" 번째 컨테이너가 nullptr 입니다.";
				ERROR_MESSAGE_W(errorMsg.c_str());
				return eResult::Fail_Nullptr;
			}

			//가져올 메쉬를 생성
			meshCont.pMesh = std::make_shared<Mesh>();

			result = meshCont.pMesh->CreateFromContainer(cont);
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"FBX로부터 메쉬 정보를 읽어오는 데 실패했습니다.");
				return eResult::Fail;
			}

			//스켈레톤 주소를 지정
			meshCont.pMesh->SetSkeleton(mSkeleton.get());

			if (nullptr != meshCont.pMesh)
			{
				//기본적으로는 컨테이너 이름을 사용
				std::fs::path strKey = cont->strName;

				//비어있을 경우 이름을 만들어준다
				if (strKey.empty())
				{
					strKey = _fileName;
					strKey.replace_extension("");
					strKey += "_";
					strKey += std::to_string(i);
				}

				//.msh로 확장자를 변경
				strKey.replace_extension(define::strKey::Ext_Mesh);
				//Key로 Mesh를 저장
				meshCont.pMesh->SetKey(strKey.string());

				//메쉬를 엔진의 포맷으로 변경해서 저장한다.
				eResult result = meshCont.pMesh->Save(strKey);

				if (eResultFail(result))
				{
					ERROR_MESSAGE_W(L"Mesh 저장 실패.");
					return result;
				}
			}


			// 메테리얼 가져오기
			for (UINT i = 0; i < cont->vecMtrl.size(); ++i)
			{
				//Material의 경우 FBX Loader에서 만들어 놨음
				// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
				std::string strKey = cont->vecMtrl[i].strMtrlName;
				std::shared_ptr<Material> pMtrl = ResMgr::Find<Material>(strKey);

				//혹시나 없을 경우 에러
				MH_ASSERT(pMtrl.get());
				
				meshCont.pMaterials.push_back(pMtrl);
			}


			mMeshContainers.push_back(meshCont);
		}//for문 끝




		//다른게 다 진행됐으면 자신도 저장
		//키값 만들고 세팅하고
		std::fs::path strKeyMeshData = _fileName;
		strKeyMeshData.replace_extension(strKey::Ext_MeshData);
		std::string strKey = strKeyMeshData.string();
		SetKey(strKey);

		//저장함수 호출
		result = Save(strKey);
		if (eResultFail(result))
		{
			return result;
		}

		//모두 문제없이 처리되었을 경우 메쉬와 재질을 ResMgr에 전부 추가한다.
		for (size_t i = 0; i < mMeshContainers.size(); ++i)
		{
			ResMgr::Insert(mMeshContainers[i].pMesh->GetKey(), mMeshContainers[i].pMesh);

			//재질은 FBX Loader에서 추가가 되어있는 상태
			//for (size_t j = 0; j < mMeshContainers[j].pMaterials.size(); ++i)
			//{
			//	ResMgr::Insert(mMeshContainers[i].pMaterials[j]->GetKey(), mMeshContainers[i].pMaterials[j]);
			//}
		}

		//전부 저장에 성공했을 경우 ResMgr에서 이 주소(MeshData)를 리소스에 추가한다
		//애초에 호출한 클래스가 ResMgr임

		return eResult::Success;
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

