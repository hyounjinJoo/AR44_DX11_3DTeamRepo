#pragma once
#include "guiResource.h"

namespace gui
{
	class Texture : public gui::GameResource
	{
	public:
		Texture();
		~Texture();

		virtual void Update() override;
	};
}
