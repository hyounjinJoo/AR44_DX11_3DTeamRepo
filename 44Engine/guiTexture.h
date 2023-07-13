#pragma once
#include "guiResource.h"

namespace gui
{
	class Texture : public gui::Resource
	{
	public:
		Texture();
		~Texture();

		virtual void Update() override;
	};
}
