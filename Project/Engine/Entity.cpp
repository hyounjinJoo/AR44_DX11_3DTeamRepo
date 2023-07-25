#include "EnginePCH.h"

#include "Entity.h"

namespace mh
{
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
