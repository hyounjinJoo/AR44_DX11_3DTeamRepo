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

		virtual define::eResult SaveJson(Json::Value* _pJVal) override;
		virtual define::eResult LoadJson(const Json::Value* _pJVal) override;

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetType(define::eColliderType _type) { mType = _type; }

	private:
		define::eColliderType mType;
	};
}


