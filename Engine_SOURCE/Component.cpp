#include "Component.h"

namespace mh
{
	Component::Component(enums::eComponentType _type)
		: mType(_type)
		, mOwner(nullptr)
	{
	}
	Component::~Component()
	{
	}
}
