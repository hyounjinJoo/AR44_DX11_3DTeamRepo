#include "PCH_Client.h"

#include "guiWidget.h"

namespace gui
{
	guiWidget::guiWidget(const std::string_view _strKey)
		: guiBase(_strKey)
	{
		SetNoChild(true);
	}

	guiWidget::~guiWidget()
	{
	}

}
