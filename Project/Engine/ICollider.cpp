#include "EnginePCH.h"

#include "ICollider.h"

namespace mh
{
	ICollider::ICollider(eColliderType _Type)
		: IComponent(eComponentType::Collider)
		, mType(_Type)
	{
	}

	ICollider::~ICollider()
	{
	}
}

