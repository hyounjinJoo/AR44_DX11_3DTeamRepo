#pragma once
#include "guiWidget.h"
#include <Engine/IRes.h>

namespace gui
{
	class IRes : public Widget
	{
	public:
		IRes();
		~IRes();

		virtual void FixedUpdate() override;
		
		void SetTarget(mh::IRes* target) { mTarget = target; }
		mh::IRes* GetTarget() const { return mTarget; }

	private:
		mh::IRes* mTarget;
	};
}
