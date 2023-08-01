#include "EnginePCH.h"
#include "Entity.h"

#include "define_Macro.h"

#include "json-cpp/json.h"

namespace mh
{
	namespace define::strKey::Json::Entity
	{
		STRKEY_DECLARE(mStrKey);
	}


	Entity::Entity()
		: mID(PtrToUint(this))
		, mStrKey()
	{
	}
	Entity::Entity(const Entity& _other)
		: mID(PtrToUint(this))
		, mStrKey(_other.mStrKey)
	{
	}
	eResult Entity::SaveJson(Json::Value* _pJson)
	{
		//nullptr 확인
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;

		//포인터를 레퍼런스 형태로 변환
		Json::Value& JVal = *(_pJson);


		//std::map 저장하듯이 [] 안에 문자열 Key를 입력하고 값을 대입해주면 저장됨.
		JVal[strKey::Json::Entity::mStrKey] = mStrKey;

		//저장이 되었을 시 Success 반환
		return eResult::Success;
	}
	eResult Entity::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;

		//포인터를 const reference 형태로 변경
		const Json::Value& JVal = *(_pJson);

		//우선 해당 string Key에 해당하는 값이 있는지 확인.
		if (JVal.isMember(strKey::Json::Entity::mStrKey))
		{
			//값이 있을 경우 값을 가져온다(string 타입이므로 string 형태로 가져옴)
			mStrKey = JVal[strKey::Json::Entity::mStrKey].asString();
		}
		//가져오려는 변수가 없어도 될때는 값이 없어도 넘어가도 됨
		//하지만 string key의 경우는 반드시 있어야 하므로 json에 값이 저장되어있지 않을 경우 에러를 반환
		else
		{
			return eResult::Fail_MissingValue;
		}

		return eResult::Success;
	}
	Entity::~Entity()
	{
	}
}

namespace gui
{
	Entity::Entity()
		: mID(PtrToUint(this))
		, mStrKey("")
	{
	}

	Entity::Entity(const char* _strKey)
		: mID(PtrToUint(this))
		, mStrKey(_strKey)
	{

	}

	Entity::Entity(const Entity& _other)
		: mID(PtrToUint(this))
		, mStrKey(_other.mStrKey)
	{
	}
	Entity::~Entity()
	{
	}
}
