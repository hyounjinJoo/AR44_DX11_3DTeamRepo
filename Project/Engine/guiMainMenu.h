#pragma once

#include "guiBase.h"

namespace gui
{
	class guiMainMenu : public guiBase
	{
	public:
		guiMainMenu();
		virtual ~guiMainMenu();

		virtual bool BeginUI() final { return ImGui::BeginMainMenuBar(); }
		virtual void UpdateUI() override;
		virtual void EndUI() final { return ImGui::EndMainMenuBar(); }

	private:

	};
}
