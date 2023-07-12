#include "yaEntity.h"

namespace ya
{
	Entity::Entity()
		: mID(reinterpret_cast<UINT32>(this))
		, mName(L"")
	{
	}
	Entity::Entity(const Entity& other)
		: mID(reinterpret_cast<UINT32>(this))
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}

namespace gui
{
	Entity::Entity()
		: mID(reinterpret_cast<UINT32>(this))
		, mName("")
	{
	}

	Entity::Entity(const char* name)
		: mID(reinterpret_cast<UINT32>(this))
		, mName(name)
	{
	}

	Entity::Entity(const Entity& other)
		: mID(reinterpret_cast<UINT32>(this))
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}