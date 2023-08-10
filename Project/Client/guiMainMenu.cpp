#include "PCH_Client.h"

#include "guiMainMenu.h"
#include <Engine/Application.h>

#include "ImGuiMgr.h"

namespace gui
{
	guiMainMenu::guiMainMenu()
		: guiBase("MainMenu")
	{
        //SetSize(ImVec2(100.0f, 100.0f));
	}

	guiMainMenu::~guiMainMenu()
	{
	}


	void guiMainMenu::UpdateUI()
	{
		if (ImGui::BeginMenu("UI Menu"))
		{
			const auto& widgets = ImGuiMgr::GetGUIs();
			for (const auto& widget : widgets)
			{
				if (this == widget)
					continue;
				else if (nullptr == widget->GetParent())
				{
					if (ImGui::MenuItem(widget->GetKey().c_str(), nullptr, widget->GetEnablePtr()))
					{
					}
				}
			}

			ImGui::Separator();

			bool p_open = GetEnable();
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
			{
				mh::Application::ShutDown();
			}

			ImGui::EndMenu();
		}
	}

}
