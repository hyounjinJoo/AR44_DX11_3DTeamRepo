#pragma once
#include "define_Enum.h"
#include "define_Macro.h"
#include "json-cpp/json-forwards.h"

#include <filesystem>

namespace mh
{
	namespace define::strKey::Json::Entity
	{
		STRKEY_DECLARE(mStrKey);
	}

	using namespace define;
	class Entity
	{
	public:
		Entity();

		Entity(const Entity& _other);
		virtual Entity* Clone() { ERROR_MESSAGE_W(L"이 클래스는 Clone할수 없습니다."); return nullptr; }

		virtual ~Entity();

		virtual eResult SaveJson(Json::Value* _pJson);
		virtual eResult LoadJson(const Json::Value* _pJson);

		void SetKey(const std::string_view _strKey) { mStrKey = _strKey; }
		const std::string& GetKey() const { return mStrKey; }
		UINT32 GetID() const { return mID; }

	private:
		std::string mStrKey;

		static UINT32 gNextID;
		const UINT32 mID;
	};
}

namespace gui
{
	class Entity
	{
	public:
		Entity();
		Entity(const char* _StrKey);
		Entity(const Entity& _other);
		virtual ~Entity();

		void SetKey(const std::string_view _StrKey) { mStrKey = _StrKey; }
		const std::string& GetKey() const { return mStrKey; }
		UINT32 GetID() const { return mID; }

	private:
		std::string mStrKey;
		const UINT32 mID;
	};
}


