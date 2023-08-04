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
		void SetSize(float2 _size) { mSize = _size; }
		void SetCenter(float2 _center) { mCenter = _center; }
		void SetRadius(float _radius) { mRadius = _radius; }
		bool IsTriiger() const { return mbTrigger; }
		uint GetID() const { return mID; }
		float3 GetPosition() const { return mPosition; }
		float2 GetSize() const { return mSize; }

	private:
		static uint gColliderNumber;
		uint mID; 
		define::eColliderType mType;
		Com_Transform* mTransform;

		float2 mSize;
		float2 mCenter;
		float3 mPosition;
		float mRadius;
		bool mbTrigger;
	};
}
