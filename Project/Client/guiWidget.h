#pragma once
#include "guiBase.h"

namespace gui
{
	//guiWidget: 딱 하나의 기능을 담당. 계층구조 가지지 않음
	class guiWidget : public guiBase
	{
	public:
		guiWidget(const std::string_view _strKey);
		virtual ~guiWidget();

	private:
	};
}
