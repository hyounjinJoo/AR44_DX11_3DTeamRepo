#pragma once
#include "guiChild.h"
#include <Engine/GameObject.h>


namespace gui
{
	class guiComponent : public guiChild
	{
	public:
		guiComponent(mh::define::eComponentType _type);
		virtual ~guiComponent();

		virtual void UpdateUI() override;
		
		void SetTarget(mh::GameObject* _target) { mTarget = _target; }
		mh::GameObject* GetTarget() { return mTarget; }

	private:
		mh::define::eComponentType mType;
		mh::GameObject* mTarget;
	};
}
