#pragma once
#include "guiWindow.h"

namespace gui
{
	class guiChild :
		public guiWindow
	{
	public:
		guiChild(const std::string_view _strKey);
		virtual ~guiChild();

		virtual bool BeginUI() final;
		virtual void UpdateUI() override {};
		virtual void EndUI() final;

	private:
		bool mbBorder;
	};
}


