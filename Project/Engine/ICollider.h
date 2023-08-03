#pragma once

#include "IComponent.h"


namespace mh
{
	class ICollider
		: public IComponent
	{
	public:
		ICollider(eColliderType _Type);
		ICollider(const ICollider& _collider) = default;
		virtual ~ICollider();

	private:
		eColliderType mType;
	};
}


