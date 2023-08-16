#include "PCH_Engine.h"

#include "ICollider.h"

namespace mh
{
	using namespace mh::define;
	ICollider::ICollider(eColliderType _Type)
		: IComponent(eComponentType::Collider)
		, mType(_Type)
	{
	}

	ICollider::~ICollider()
	{
	}
}

