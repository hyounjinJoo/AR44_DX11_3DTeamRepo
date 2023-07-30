#include "EnginePCH.h"
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

	namespace stdfs = std::filesystem;

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
	eResult Prefab::Save(const std::filesystem::path& _FilePath)
	{
		if (nullptr == mPrefab)
		{
			ERROR_MESSAGE_W(L"저장할 프리팹이 지정되지 않았습니다.");
			return eResult::Fail_InValid;
		}

		//혹시나 경로에 해당하는 폴더 경로가 없을 시에 throw error가 돼서 프로그램이 강제 종료되므로
		//폴더 경로를 직접 만들어준다.
		stdfs::path SavePath = PathMgr::GetRelativeResourcePath(GetResType());
		if (false == stdfs::exists(SavePath))
		{
			if (false == stdfs::create_directories(SavePath))
			{
				ERROR_MESSAGE_W(L"파일 저장을 위한 디렉토리 생성에 실패했습니다.");
				return eResult::Fail_OpenFile;
			}
		}
		
		SavePath /= _FilePath;
		SavePath.replace_extension(".json");
		std::ofstream SaveFile(SavePath);
		if (false == SaveFile.is_open())
		{
			ERROR_MESSAGE_W(L"저장 파일 생성에 실패했습니다.");
			return eResult::Fail_OpenFile;
		}

		//자신(Resource)의 데이터 저장
		Json::Value MyJVal{};
		eResult result = SaveJson(&MyJVal);
		if (eResultFail(result))
		{
			return eResult::Fail_Json;
		}

		//프리팹(GameObject)의 데이터 저장
		Json::Value PrefabJVal{};
		result = mPrefab->SaveJson(&PrefabJVal);
		if(eResultFail(result))
		{
			//json 저장에 실패 시 파일을 작성하지 않는다.
			SaveFile.close();
			return eResult::Fail_Json;
		}

		//게임오브젝트의 json 데이터를 나의 json 데이터에 붙인다(Object Value)
		MyJVal[strKey::Json::Prefab::mPrefab] = PrefabJVal;
		
		//저장
		SaveFile << MyJVal;
		SaveFile.close();

		return eResult::Success;
	}
	eResult Prefab::Load(const std::filesystem::path& _FilePath)
	{
		//로드 시에는 게임오브젝트에서 무조건 포인터를 관리해야 하므로 삭제 방지를 꺼준다.
		mbDontDelete = false;
		SAFE_DELETE(mPrefab);

		stdfs::path LoadPath = PathMgr::GetRelativeResourcePath(GetResType());
		LoadPath /= _FilePath;

		if (false == stdfs::exists(LoadPath))
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
		mPrefab = new GameObject;
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

