#pragma once
#include "guiWidget.h"
#include "Resource.h"

namespace gui
{
	class Resource : public Widget
	{
	public:
		Resource();
		~Resource();

		virtual void FixedUpdate() override;

		void SetTarget(ya::Resource* target) { mTarget = target; }
		ya::Resource* GetTarget() const { return mTarget; }

	private:
		ya::Resource* mTarget;
	};
}
