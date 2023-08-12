#pragma once
#include "guiComponent.h"

namespace gui
{
	class guiInspector;
	class guiCom_Transform : public guiComponent
	{
	public:
		guiCom_Transform();
		virtual ~guiCom_Transform();

		virtual void Update() override;
		virtual void UpdateUI() override;

	private:
		float3 mPosisition;
		float3 mRotation;
		float3 mScale;
	};

}
