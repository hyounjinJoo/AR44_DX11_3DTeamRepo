#pragma once
#include "guiComponent.h"

namespace gui
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		ya::math::Vector3 mPosisition;
		ya::math::Vector3 mRotation;
		ya::math::Vector3 mScale;
	};

}
