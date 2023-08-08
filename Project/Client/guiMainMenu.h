#pragma once
#include "guiWidget.h"

namespace gui
{
	class MainMenu : public Widget
	{
	public:
		MainMenu();
		virtual ~MainMenu();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:

	};
}
