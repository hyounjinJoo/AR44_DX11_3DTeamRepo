#pragma once

#include "IComponent.h"

#include "SimpleMath.h"

namespace mh
{
	using namespace mh::math;

	class GameObject;
	class Com_Transform : public IComponent
	{
	public:
		Com_Transform();
		virtual ~Com_Transform();

		virtual void Initialize() final {};
		virtual void Update() final {};
		virtual void FixedUpdate();
		virtual void Render() {};

		void SetConstantBuffer();
		

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
		Vector3 mFoward;
		Vector3 mRight;
		Vector3 mUp;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		Matrix mWorld;
	};
}