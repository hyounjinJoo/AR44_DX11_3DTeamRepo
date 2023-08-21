#pragma once
#include "ICollider.h"

namespace mh
{
	class ICollider3D : public ICollider
	{
	public:
		ICollider3D();
		ICollider3D(const ICollider3D& _collider);
		CLONE(ICollider3D);
		
		virtual ~ICollider3D();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		//virtual void Init() override;
		//virtual void Update() override;
		virtual void FixedUpdate() override { MH_ASSERT(false); }; // 구현 안함

		void SetType(define::eColliderType _type) { mType = _type; }

	private:
		define::eColliderType mType;
	};
}


