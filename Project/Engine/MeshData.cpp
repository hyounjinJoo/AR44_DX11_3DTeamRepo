#include "PCH_Engine.h"
#include "MeshData.h"

#include "define_Util.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "FBXLoader.h"
#include "GameObject.h"
#include "Com_Renderer_Mesh.h"
#include "Com_Animator3D.h"
#include "define_Util.h"

#include "PathMgr.h"
#include "json-cpp/json.h"
#include "Skeleton.h"

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

	eResult MeshData::Load(const std::filesystem::path& _path)
	{
		std::fs::path fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		fullPath /= _path;

		std::string ext = StringConv::UpperCaseReturn(_path.extension().string());

		//FBX일 경우에는 FBXLoader를 통해서 가져온다.
		if (".FBX" == ext)
		{
			FBXLoader loader{};
			loader.Init();
			eResult result = loader.LoadFbx(fullPath);
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"FBX 불러오기 실패.");
				return result;
			}

			// 메쉬 가져오기
			std::shared_ptr<Mesh> pMesh = nullptr;
			pMesh = Mesh::CreateFromContainer(loader);

			// ResMgr 에 메쉬 등록
			if (nullptr != pMesh)
			{	
				std::fs::path strKey = _path;
				//.msh로 확장자를 변경
				strKey.replace_extension(define::strKey::Ext_Mesh);
				//Key로 Mesh를 저장
				pMesh->SetKey(strKey.string());

				//메쉬를 엔진의 포맷으로 변경해서 저장한다.
				eResult result = pMesh->Save(strKey);
				
				if (eResultFail(result))
				{
					ERROR_MESSAGE_W(L"Mesh 저장 실패.");
					return result;
				}
				
				//성공 시 ResMgr에 넣는다.
				ResMgr::Insert(strKey.string(), pMesh);
			}

			std::vector<std::shared_ptr<Material>> vecMtrl;

			// 메테리얼 가져오기
			for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
			{
				// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
				std::string strKey = loader.GetContainer(0).vecMtrl[i].strMtrlName;
				std::shared_ptr<Material> pMtrl = ResMgr::Find<Material>(strKey);
				MH_ASSERT(pMtrl.get());


				vecMtrl.push_back(pMtrl);
			}

			mMesh = pMesh;
			mMaterials = vecMtrl;

			//다른게 다 진행됐으면 자신도 저장
			//키값 만들고 세팅하고
			std::fs::path strKeyMeshData = _path;
			strKeyMeshData.replace_extension(strKey::Ext_MeshData);
			std::string strKey = strKeyMeshData.string();
			SetKey(strKey);

			//저장함수 호출
			result = Save(_path);
			if (eResultFail(result))
			{
				return result;
			}

			//성공 시 ResMgr가 이걸 리소스 목록에 등록해줄것임
		}

		//그렇지 않을 경우 json을 통해 로드
		else
		{
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

		//둘중 하나라도 없을 경우 저장 불가
		if (nullptr == mMesh || mMaterials.empty())
			return eResult::Fail_InValid;

		Json::Value& jVal = *_pJson;

		Json::MH::SaveStrKey(_pJson, JSON_KEY_PAIR(mMesh));

		Json::MH::SaveStrKeyVector(_pJson, JSON_KEY_PAIR(mMaterials));

		return eResult::Success;
	}


	eResult MeshData::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;
		eResult result = IRes::LoadJson(_pJson);
		if (eResultFail(result))
			return result;

		const Json::Value& jVal = *_pJson;

		{
			std::string strKey = Json::MH::LoadStrKey(_pJson, JSON_KEY_PAIR(mMesh));
			mMesh = ResMgr::Load<Mesh>(strKey);
			if (nullptr == mMesh)
			{
				return eResult::Fail_Create;
			}
		}
		
		{
			mMaterials.clear();
			const auto& strKeys = Json::MH::LoadStrKeyVector(_pJson, JSON_KEY_PAIR(mMaterials));
			for (size_t i = 0; i < strKeys.size(); ++i)
			{
				std::shared_ptr<Material> material = nullptr;
				if (false == strKeys[i].empty())
				{
					material = ResMgr::Load<Material>(strKeys[i]);
				}

				mMaterials.push_back(material);
			}
		}

		return eResult::Success;
	}


	GameObject* MeshData::Instantiate()
	{
		GameObject* pNewObj = new GameObject;
		Com_Renderer_Mesh* Renderer = pNewObj->AddComponent<Com_Renderer_Mesh>();

		Renderer->SetMesh(mMesh);

		for (UINT i = 0; i < mMaterials.size(); ++i)
		{
			Renderer->SetMaterial(mMaterials[i], i);
		}

		if (mMesh->IsAnimMesh())
		{
			Com_Animator3D* Animator3D = pNewObj->AddComponent<Com_Animator3D>();

			Animator3D->SetBones(mMesh->GetBones());
			Animator3D->SetAnimClip(mMesh->GetAnimClip());
		}

		return pNewObj;
	}
}

