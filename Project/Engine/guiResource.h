#pragma once
#include "guiChild.h"
#include "IRes.h"

namespace gui
{
	class guiResource : public guiChild
	{
	public:
		guiResource(mh::eResourceType _resType);
		virtual ~guiResource();

		void UpdateUI() override;
		
		void SetTarget(mh::IRes* target) { mTarget = target; }
		mh::IRes* GetTarget() const { return mTarget; }

	private:
		mh::IRes* mTarget;
	};
}
