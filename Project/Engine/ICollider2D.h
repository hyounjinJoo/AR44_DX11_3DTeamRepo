#pragma once
#include "ICollider.h"


namespace mh
{
	class Com_Transform;
	class ICollider2D : public ICollider
	{
	public:
		ICollider2D();
		ICollider2D(const ICollider2D& _collider);
		CLONE(ICollider2D);

		virtual ~ICollider2D();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(ICollider2D* _collider);
		virtual void OnCollisionStay(ICollider2D* _collider);
		virtual void OnCollisionExit(ICollider2D* _collider);

		virtual void OnTriggerEnter(ICollider2D* _collider);
		virtual void OnTriggerStay(ICollider2D* _collider);
		virtual void OnTriggerExit(ICollider2D* _collider);

		void SetType(define::eColliderType _type) { mType = _type; }
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
		define::eColliderType mType;
		Com_Transform* mTransform;

		math::Vector2 mSize;
		math::Vector2 mCenter;
		math::Vector3 mPosition;
		float mRadius;
		bool mbTrigger;
	};
}
