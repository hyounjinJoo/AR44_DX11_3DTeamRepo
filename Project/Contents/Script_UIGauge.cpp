#include "PCH_Contents.h"
#include "Script_UIGauge.h"

#include <Engine/Com_Renderer_UIBase.h>

namespace mh
{
	Script_UIGauge::Script_UIGauge()
		: Script_UIBase(eUIType::Gauge)
		, mGaugeRemain()
	{
	}

	Script_UIGauge::~Script_UIGauge()
	{
	}

	void Script_UIGauge::Update()
	{
		if (mUIComponent)
		{
			//레퍼런스로 값을 받아오기 때문에 바로 수정하면 바로 반영됨
			tCB_UniformData& data = mUIComponent->GetUIUniformData();
			data.UI_GAUGE_REMAIN = mGaugeRemain;
		}
	}

}

