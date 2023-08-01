#include "EnginePCH.h"

#include "IComponent.h"

namespace mh
{
	IComponent::IComponent(define::eComponentType _type)
		: mType(_type)
		, mOwner(nullptr)
	{
	}

	IComponent::IComponent(const IComponent& _other)
		: Entity(_other)
		, mType(_other.mType)
		, mOwner(nullptr)
	{
	}

	IComponent::~IComponent()
	{
	}
}
