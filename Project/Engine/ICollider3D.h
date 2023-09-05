#pragma once
#include "ICollider.h"

namespace mh
{
	class Mesh;
	class Material;

	class ICollider3D : public ICollider
	{
	public:
		ICollider3D();
		ICollider3D(const ICollider3D& _collider);
		CLONE(ICollider3D);
		
		virtual ~ICollider3D();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override; 

		void OnCollisionEnter(ICollider3D* _otherCollider);
		void OnCollisionStay(ICollider3D* _otherCollider);
		void OnCollisionExit(ICollider3D* _otherCollider);

		void OnTriggerEnter(ICollider3D* _otherCollider);
		void OnTriggerStay(ICollider3D* _otherCollider);
		void OnTriggerExit(ICollider3D* _otherCollider);

		bool IsCollision() { return mCollisionCount; }
		void ClearCollisionCount() { mCollisionCount = 0; }

		void SetType(define::eColliderType _type) { mType = _type; }

	private:
		define::eColliderType mType;

		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;


		int mCollisionCount;
	};
}


