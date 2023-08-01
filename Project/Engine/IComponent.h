#pragma once
#include "Entity.h"

#include "define_Component.h"
#include "SimpleMath.h"

namespace mh
{
	using namespace mh::define;
	class GameObject;
	class IComponent : public Entity
	{
	public:
		IComponent(eComponentType _type);

		IComponent(const IComponent& _other);

		virtual ~IComponent();

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void Render() = 0;

		eComponentType GetComType() const { return mType; };

		GameObject* GetOwner() const { return mOwner; }
		void SetOwner(GameObject* _owner) { mOwner = _owner; }

	private:
		const eComponentType mType;
		GameObject* mOwner;
	};
}
