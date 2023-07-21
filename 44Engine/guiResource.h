#pragma once
#include "guiWidget.h"
#include "GameResource.h"

namespace gui
{
	class GameResource : public Widget
	{
	public:
		GameResource();
		~GameResource();

		virtual void FixedUpdate() override;
		
		void SetTarget(mh::GameResource* target) { mTarget = target; }
		mh::GameResource* GetTarget() const { return mTarget; }

	private:
		mh::GameResource* mTarget;
	};
}
