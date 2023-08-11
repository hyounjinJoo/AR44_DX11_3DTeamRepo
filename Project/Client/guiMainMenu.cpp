#include "PCH_Client.h"

#include "guiMainMenu.h"
#include <Engine/Application.h>

#include "guiMgr.h"

namespace gui
{
	guiMainMenu::guiMainMenu()
		: guiBase(strKey::MainMenu)
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
			const auto& guis = guiMgr::GetGUIs();
			for (const auto& widget : guis)
			{
				if (this == widget.second)
					continue;
				else if (nullptr == widget.second->GetParent())
				{
					if (ImGui::MenuItem(widget.second->GetKey().c_str(), nullptr, widget.second->GetEnablePtr()))
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
