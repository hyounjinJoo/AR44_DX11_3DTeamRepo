#pragma once
#include "guiChild.h"
#include <Engine/IRes.h>

namespace gui
{
	class IRes : public guiChild
	{
	public:
		IRes(const std::string_view _strKey);
		virtual ~IRes();

		void UpdateUI() override;
		
		void SetTarget(mh::IRes* target) { mTarget = target; }
		mh::IRes* GetTarget() const { return mTarget; }

	private:
		mh::IRes* mTarget;
	};
}
