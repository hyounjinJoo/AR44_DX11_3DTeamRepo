#pragma once
#include "EnginePCH.h"

namespace mh
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

		//TODO: Clone 함수 추가
		Entity(const Entity& _other);

		void SetKey(const std::string_view _strKey) { mStrKey = _strKey; }
		const std::string& GetKey() const { return mStrKey; }
		UINT32 GetID() const { return mID; }

	private:
		std::string mStrKey;
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


