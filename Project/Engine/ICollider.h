#pragma once

#include "IComponent.h"


namespace mh
{
	class ICollider
		: public IComponent
	{
	public:
		ICollider(eColliderType _Type);
		virtual ~ICollider();

	private:
		eColliderType mType;
	};
}


