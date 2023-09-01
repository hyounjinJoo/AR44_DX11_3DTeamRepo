#include "PCH_Engine.h"
#include "IRes.h"

#include "PathMgr.h"

#include "json-cpp/json.h"

namespace mh
{
	namespace define::strKey::Json::IRes
	{
		STRKEY_DECLARE(mType);
	}

	

	IRes::IRes(eResourceType _type)
		: mType(_type)
		, mbEngineDefaultRes()
	{
	}
	IRes::~IRes()
	{
	}

	eResult IRes::Save(const std::fs::path& _filePath, const std::fs::path& _basePath)
	{
		mBasePath = _basePath;
		return eResult::Success;
	}

	eResult IRes::Load(const std::fs::path& _filePath, const std::fs::path& _basePath)
	{
		mBasePath = _basePath;
		return eResult::Success;
	}

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
		Jval[strKey::Json::IRes::mType] = strKey::ArrResName[(int)mType];


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
			if (strKey::ArrResName[(int)mType] != ResourceName)
			{
				ERROR_MESSAGE_W(L"읽어들인 json 파일이 리소스 타입과 일치하지 않습니다.");
				return eResult::Fail_InValid;
			}
		}


		return eResult::Success;
	}

}
