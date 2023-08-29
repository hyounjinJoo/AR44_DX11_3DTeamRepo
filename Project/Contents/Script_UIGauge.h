#pragma once
#include "Script_UIBase.h"

namespace mh
{
	class Script_UIGauge :
		public Script_UIBase
	{
	public:
		Script_UIGauge();
		virtual ~Script_UIGauge();

		virtual void Update() override;

		void SetGaugeRemain(float _remain) { mGaugeRemain = _remain; };
		void SetGaugeRemainPercent(float _remain) { mGaugeRemain = _remain / 100.f; }
		float GetGaugeRemain() const { return mGaugeRemain; }

	private:
		float mGaugeRemain;
	};
}


