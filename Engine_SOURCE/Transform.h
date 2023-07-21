#pragma once
#include "IComponent.h"

using namespace mh::math;
namespace mh
{
	class Transform : public IComponent
	{
	public:
		Transform();
		virtual ~Transform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetConstantBuffer();
		
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