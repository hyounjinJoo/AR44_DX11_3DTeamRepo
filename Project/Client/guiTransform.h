#pragma once
#include "guiComponent.h"

namespace gui
{
	class guiTransform : public guiComponent
	{
	public:
		guiTransform();
		virtual ~guiTransform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetTarget(mh::GameObject* _target) { mTarget = _target; }
		mh::GameObject* GetTarget() { return mTarget; }

	private:
		mh::define::eComponentType mType;
		mh::GameObject* mTarget;

		mh::math::Vector3 mPosisition;
		mh::math::Vector3 mRotation;
		mh::math::Vector3 mScale;
	};

}
