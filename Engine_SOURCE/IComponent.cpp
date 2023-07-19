#include "IComponent.h"

namespace mh
{
	IComponent::IComponent(enums::eComponentType _type)
		: mType(_type)
		, mOwner(nullptr)
	{
	}
	IComponent::~IComponent()
	{
	}
}
