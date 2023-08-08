#include "PCH_Client.h"

#include "guiMainMenu.h"
#include <Engine/Application.h>

#include "ImGuiMgr.h"

namespace gui
{
	MainMenu::MainMenu()
		: Widget("MainMenu")
	{
        //SetSize(ImVec2(100.0f, 100.0f));
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::FixedUpdate()
	{

	}

	void MainMenu::Update()
	{
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
				const auto& widgets = ImGuiMgr::GetWidgets();
				for (const auto& widget : widgets)
				{
					if (this == widget.second)
						continue;
					else if (nullptr == widget.second->GetParent())
					{
						if (ImGui::MenuItem(widget.second->GetKey().c_str()))
						{
							eState state = widget.second->GetState();
							switch (state)
							{
							case gui::Widget::eState::Paused:
								widget.second->SetState(eState::Active);
								break;
							case gui::Widget::eState::Active:
								widget.second->SetState(eState::Paused);
								break;
							case gui::Widget::eState::Dead:
								break;
							default:
								break;
							}
						}
					}
				}


                ImGui::Separator();

                bool p_open = (bool)GetState();
				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				{
					mh::Application::ShutDown();
				}

                ImGui::EndMenu();
            }
           
            ImGui::EndMainMenuBar();
        }
	}

	void MainMenu::LateUpdate()
	{
	}

}
