#include "PCH_Engine.h"
#include "guiEntity.h"
#include "json-cpp/json.h"

namespace gui
{
	UINT32 guiEntity::gIDNext{};

	guiEntity::guiEntity(const std::string_view _strName)
		: mID(++gIDNext)
		, mName(_strName)
		, mStrKey(_strName)
		, mbSaveEnable()
	{
	}


	guiEntity::~guiEntity()
	{
	}
	
	//mh::eResult guiEntity::SaveJson(Json::Value* _pJval)
	//{
	//	if (nullptr == _pJval)
	//		return mh::eResult::Fail_Nullptr;

	//	Json::Value& jVal = (*_pJval);
	//	Json::MH::SaveValue(_pJval, JSON_KEY_PAIR(mName));

	//	return mh::eResult::Success;
	//}

	//mh::eResult guiEntity::LoadJson(const Json::Value* _pJval)
	//{
	//	if (nullptr == _pJval)
	//		return mh::eResult::Fail_Nullptr;

	//	const Json::Value& jVal = (*_pJval);
	//	Json::MH::LoadValue(_pJval, JSON_KEY_PAIR(mName));

	//	return mh::eResult::Success;
	//}
}


