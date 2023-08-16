#pragma once
#include "guiResource.h"

namespace gui
{
	class guiTexture : public gui::guiResource
	{
	public:
		guiTexture();
		virtual ~guiTexture();

		virtual void UpdateUI() override;
	};
}
