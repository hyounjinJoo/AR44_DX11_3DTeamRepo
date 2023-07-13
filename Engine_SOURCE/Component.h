#pragma once
#include "Math.h"
#include "Entity.h"

namespace mh
{
	class GameObject;
	class Component : public Entity
	{
	public:
		Component(enums::eComponentType _type);
		virtual ~Component();

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void Render() = 0;

		enums::eComponentType GetOrder() const { return mType; };

		GameObject* GetOwner() const { return mOwner; }
		void SetOwner(GameObject* _owner) { mOwner = _owner; }

	private:
		const enums::eComponentType mType;
		GameObject* mOwner;
	};
}
