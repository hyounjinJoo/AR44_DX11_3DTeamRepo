#include "PCH_Engine.h"
#include "Prefab.h"

#include "PathMgr.h"
#include "json-cpp\json.h"
#include "GameObject.h"

namespace mh
{
	namespace define::strKey::Json::Prefab
	{
		STRKEY_DECLARE(mPrefab);
	}

	

	Prefab::Prefab()
		: IRes(eResourceType::Prefab)
		, mPrefab()
		, mbDontDelete()
	{
	}

	Prefab::~Prefab()
	{
		if (false == mbDontDelete)
		{
			SAFE_DELETE(mPrefab);
		}
	}
	eResult Prefab::Save(const std::filesystem::path& _fileName)
	{
		if (nullptr == mPrefab)
		{
			ERROR_MESSAGE_W(L"저장할 프리팹이 지정되지 않았습니다.");
			return eResult::Fail_InValid;
		}

		//키 값을 json을 제외한 파일명으로 설정
		{
			std::fs::path strKey = _fileName;
			strKey.replace_extension("");
			mPrefab->SetKey(strKey.string());
		}

		//혹시나 경로에 해당하는 폴더 경로가 없을 시에 throw error가 돼서 프로그램이 강제 종료되므로
		//폴더 경로를 직접 만들어준다.
		std::fs::path SavePath = PathMgr::GetContentPathRelative(GetResType());
		if (false == std::fs::exists(SavePath))
		{
			if (false == std::fs::create_directories(SavePath))
			{
				ERROR_MESSAGE_W(L"파일 저장을 위한 디렉토리 생성에 실패했습니다.");
				return eResult::Fail_OpenFile;
			}
		}
		
		SavePath /= _fileName;
		SavePath.replace_extension(".json");
		std::ofstream SaveFile(SavePath);
		if (false == SaveFile.is_open())
		{
			ERROR_MESSAGE_W(L"저장 파일 생성에 실패했습니다.");
			return eResult::Fail_OpenFile;
		}

		//프리팹(GameObject)의 데이터 저장
		Json::Value PrefabJVal{};
		eResult result = mPrefab->SaveJson(&PrefabJVal);
		if(eResultFail(result))
		{
			//json 저장에 실패 시 파일을 작성하지 않는다.
			SaveFile.close();
			return eResult::Fail_Json;
		}
		
		//저장
		SaveFile << PrefabJVal;
		SaveFile.close();

		return eResult::Success;
	}
	eResult Prefab::Load(const std::filesystem::path& _FilePath)
	{
		if (nullptr == mPrefab)
		{
			mPrefab = new GameObject;
		}
		//초기 상태의 GameObject가 아닐 경우에는 에러를 발생시킨다.
		if (false == mPrefab->GetKey().empty())
		{
			ERROR_MESSAGE_W(L"초기화되어있지 않은 Prefab에 데이터를 등록하려고 시도했습니다.");
			return eResult::Fail;
		}

		std::fs::path LoadPath = PathMgr::GetContentPathRelative(GetResType());
		LoadPath /= _FilePath;
		LoadPath.replace_extension(".json");

		if (false == std::fs::exists(LoadPath))
		{
			ERROR_MESSAGE_W(L"로드할 파일을 찾지 못했습니다.");
			return eResult::Fail_OpenFile;
		}

		std::ifstream LoadFile(LoadPath);
		if (false == LoadFile.is_open())
		{
			ERROR_MESSAGE_W(L"파일을 여는 데 실패했습니다.");
			return eResult::Fail_OpenFile;
		}

		//파일에서 데이터를 읽어온다.
		Json::Value LoadJVal{};
		LoadFile >> LoadJVal;
		LoadFile.close();

		//내 데이터를 로드한다.
		eResult result = LoadJson(&LoadJVal);
		if (eResultFail(result))
		{
			return eResult::Fail_Json;
		}

		//게임오브젝트 데이터를 로드한다.
		if (false == LoadJVal.isMember(strKey::Json::Prefab::mPrefab))
		{
			ERROR_MESSAGE_W(L"Prefab Data를 찾지 못했습니다.");
			return eResult::Fail_Json;
		}

		const Json::Value& PrefabJVal = LoadJVal[strKey::Json::Prefab::mPrefab];
		result = mPrefab->LoadJson(&PrefabJVal);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"프리팹을 불러오는 데 실패했습니다.");
			SAFE_DELETE(mPrefab);
			return eResult::Fail_Json;
		}

		return eResult::Success;
	}


	GameObject* Prefab::Instantiate()
	{
		return nullptr;
	}
}

