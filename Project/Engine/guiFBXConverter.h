#pragma once
#include "guiWindow.h"

namespace gui
{
	class guiFBXConverter :
		public guiWindow
	{
	public:
		guiFBXConverter();
		virtual ~guiFBXConverter();

		virtual void Init() override;
		virtual void Update() override;
		virtual void UpdateUI() override;

	private:

	};
}


