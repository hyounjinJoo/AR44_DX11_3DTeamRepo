#include "EnginePCH.h"

#include "Entity.h"

namespace mh
{
	Entity::Entity()
		: mID(PtrToUint(this))
		, mName(L"")
	{
	}
	Entity::Entity(const Entity& _other)
		: mID(PtrToUint(this))
		, mName(_other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}

namespace gui
{
	Entity::Entity()
		: mID(PtrToUint(this))
		, mName("")
	{
	}

	Entity::Entity(const char* _name)
		: mID(PtrToUint(this))
		, mName(_name)
	{

	}

	Entity::Entity(const Entity& _other)
		: mID(PtrToUint(this))
		, mName(_other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}