#include "EnginePCH.h"
#include "IRes.h"

#include "PathMgr.h"

#include "json-cpp/json.h"

namespace mh
{
	namespace define::strKey::Json::IRes
	{
		STRKEY_DECLARE(mType);
	}

	namespace stdfs = std::filesystem;

	IRes::IRes(eResourceType _type)
		: mType(_type)
		, mbEngineDefaultRes()
	{
	}
	IRes::~IRes()
	{
	}

	//eResult IRes::Save(const std::filesystem::path& _path)
	//{
	//	stdfs::path FullPath = PathMgr::GetRelativeResourcePath(mType);
	//	if (false == stdfs::exists(FullPath))
	//	{
	//		if (false == stdfs::create_directories(FullPath))
	//		{
	//			ERROR_MESSAGE_W(L"저장을 위한 디렉토리 생성에 실패했습니다.");
	//			return eResult::Fail_OpenFile;
	//		}
	//	}

	//	FullPath /= _path;
	//	std::ofstream OutFile(FullPath);
	//	if (false == OutFile.is_open())
	//	{
	//		ERROR_MESSAGE_W(L"파일 열기에 실패했습니다.");
	//		return eResult::Fail_OpenFile;
	//	}
	//	
	//	Json::Value JVal{};

	//	eResult Result = SaveJson(&JVal);
	//	if (eResult::Success != SaveJson(&JVal))
	//	{
	//		ERROR_MESSAGE_W(L"Json 파일 저장에 실패했습니다.");
	//		return eResult::Fail_Json;
	//	}

	//	//"<<" 연산자를 통해 메모리에 저장된 Json 데이터를 파일로 옮길 수 있다.
	//	OutFile << JVal;

	//	//다 옮겼으면 파일을 닫하준다.
	//	OutFile.close();

	//	return eResult::Success;
	//}
	eResult IRes::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = Entity::SaveJson(_pJVal);
		
		if (eResult::Success != result)
		{
			return result;
		}

		Json::Value& Jval = *(_pJVal);

		//리소스 타입을 문자열 형태로 가져와서 저장
		Jval[strKey::Json::IRes::mType] = ArrResName[(int)mType];


		return eResult::Success;
	}
	eResult IRes::LoadJson(const Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = Entity::LoadJson(_pJVal);
		if (eResult::Success != result)
		{
			return result;
		}

		const Json::Value& Jval = *(_pJVal);

		//저장된 json 파일과 로드할 json 파일의 타입이 일치하는지만 확인.
		if (Jval.isMember(strKey::Json::IRes::mType))
		{
			std::string ResourceName = Jval[strKey::Json::IRes::mType].asString();
			if (ArrResName[(int)mType] != ResourceName)
			{
				ERROR_MESSAGE_W(L"읽어들인 json 파일이 리소스 타입과 일치하지 않습니다.");
				return eResult::Fail_InValid;
			}
		}


		return eResult::Success;
	}
}
