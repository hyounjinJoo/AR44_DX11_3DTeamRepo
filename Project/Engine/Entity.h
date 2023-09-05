#pragma once
#include "define_Enum.h"
#include "define_Macro.h"
#include "json-cpp/json-forwards.h"
#include <string>
#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}


namespace mh
{
	using namespace define;
	namespace define::strKey::Json::Entity
	{
		STRKEY_DECLARE(mStrKey);
	}

	class Entity
	{
	public:
		Entity();

		Entity(const Entity& _other);
		virtual Entity* Clone() { ERROR_MESSAGE_W(L"이 클래스는 Clone할수 없습니다."); return nullptr; }

		Entity(Entity&& _move);

		virtual ~Entity();

		virtual eResult SaveJson(Json::Value* _pJson);
		virtual eResult LoadJson(const Json::Value* _pJson);

		void SetKey(const std::string_view _strKey) { mStrKey = _strKey; }
		const std::string& GetKey() const { return mStrKey; }
		UINT32 GetID() const { return mID; }

	private:
		std::string mStrKey;
		

		static UINT32 gIDNext;
		const UINT32 mID;
	};
}






