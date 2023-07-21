#pragma once
#include "guiComponent.h"

namespace gui
{
	class Transform : public IComponent
	{
	public:
		Transform();
		~Transform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		mh::math::Vector3 mPosisition;
		mh::math::Vector3 mRotation;
		mh::math::Vector3 mScale;
	};

}
