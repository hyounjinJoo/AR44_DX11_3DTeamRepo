#pragma once
#include "Component.h"
#include "Transform.h"

namespace mh
{
	class Collider2D : public Component
	{
	public:
		Collider2D();
		~Collider2D();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* _collider);
		virtual void OnCollisionStay(Collider2D* _collider);
		virtual void OnCollisionExit(Collider2D* _collider);

		virtual void OnTriggerEnter(Collider2D* _collider);
		virtual void OnTriggerStay(Collider2D* _collider);
		virtual void OnTriggerExit(Collider2D* _collider);

		void SetType(enums::eColliderType _type) { mType = _type; }
		void SetSize(math::Vector2 _size) { mSize = _size; }
		void SetCenter(math::Vector2 _center) { mCenter = _center; }
		void SetRadius(float _radius) { mRadius = _radius; }
		bool IsTriiger() const { return mbTrigger; }
		UINT GetID() const { return mID; }
		math::Vector3 GetPosition() const { return mPosition; }
		math::Vector2 GetSize() const { return mSize; }

	private:
		static UINT gColliderNumber;
		UINT mID; 
		enums::eColliderType mType;
		Transform* mTransform;

		math::Vector2 mSize;
		math::Vector2 mCenter;
		math::Vector3 mPosition;
		float mRadius;
		bool mbTrigger;
	};
}
