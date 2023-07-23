#pragma once
#include "EnginePCH.h"

namespace mh
{
	class Entity
	{
	public:
		Entity();
		Entity(const Entity& _other);
		virtual ~Entity();

		void SetName(const std::wstring& _name) { mName = _name; }
		const std::wstring& GetName() const { return mName; }
		UINT32 GetID() const { return mID; }

	private:
		std::wstring mName;
		const UINT32 mID;
	};
}

namespace gui
{
	class Entity
	{
	public:
		Entity();
		Entity(const char* _name);
		Entity(const Entity& _other);
		virtual ~Entity();

		void SetName(const std::string& _name) { mName = _name; }
		const std::string& GetName() const { return mName; }
		UINT32 GetID() const { return mID; }

	private:
		std::string mName;
		const UINT32 mID;

	};
}


