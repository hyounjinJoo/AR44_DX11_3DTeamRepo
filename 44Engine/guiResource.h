#pragma once
#include "guiWidget.h"
#include "yaResource.h"


namespace gui
{
	class Resource : public Widget
	{
	public:
		Resource();
		~Resource();

		virtual void FixedUpdate() override;

		void SetTarget(ya::Resource* target) { mTarget = target; }
		ya::Resource* GetTarget() { return mTarget; }

	private:
		ya::Resource* mTarget;
	};
}
