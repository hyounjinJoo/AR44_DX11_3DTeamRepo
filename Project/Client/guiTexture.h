#pragma once
#include "guiResource.h"

namespace gui
{
	class Texture : public gui::IRes
	{
	public:
		Texture();
		~Texture();

		virtual void Update() override;
	};
}
