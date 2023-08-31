#pragma once
#include "Entity.h"

#include "define_Component.h"
#include "SimpleMath.h"

namespace mh
{
	class GameObject;
	class IComponent : public Entity
	{
	public:
		IComponent(define::eComponentType _type);

		IComponent(const IComponent& _other);
		CLONE_DISABLE(IComponent);

		virtual ~IComponent();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		virtual void Init() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};

		define::eComponentType GetComType() const { return mType; };

		void SetComTypeID(UINT32 _comTypeID) { mComTypeID = _comTypeID; }
		UINT32 GetComTypeID() const { return mComTypeID; };

		GameObject* GetOwner() const { return mOwner; }
		void SetOwner(GameObject* _owner) { mOwner = _owner; }

		bool IsPhysicsObject();

	private:
		const define::eComponentType mType;
		UINT32 mComTypeID;
		GameObject* mOwner;
	};
}
