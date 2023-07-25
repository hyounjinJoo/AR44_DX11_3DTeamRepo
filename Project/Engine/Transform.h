#pragma once

#include "Entity.h"

#include "SimpleMath.h"

namespace mh
{
	using namespace mh::math;

	class GameObject;
	class Transform : public Entity
	{
	public:
		Transform();
		virtual ~Transform();

		//void Initialize();
		//void Update();
		void FixedUpdate();
		//void Render() {};

		void SetConstantBuffer();
		
		void SetOwner(GameObject* _pOwner) { mOwner = _pOwner; }
		void SetParent(Transform* _parent) { mParent = _parent; }
		Transform* GetParent() { return mParent; }

		Vector3 GetPosition() { return mPosition; };
		Vector3 GetRotation() { return mRotation; };
		Vector3 GetScale() { return mScale; };

		void SetPosition(const Vector3& _position) { mPosition = _position; };
		void SetRotation(const Vector3& _degree) { mRotation = _degree; };
		void SetScale(const Vector3& _scale) { mScale = _scale; };

		Vector3 Foward() { return mFoward; }
		Vector3 Right() { return mRight; }
		Vector3 Up() { return mUp; }

		Matrix& GetWorldMatrix() { return mWorld; }

	private:
		GameObject* mOwner;
		Transform* mParent;
		Vector3 mFoward;
		Vector3 mRight;
		Vector3 mUp;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		Matrix mWorld;
	};
}
