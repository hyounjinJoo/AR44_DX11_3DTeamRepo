#include "PCH_Client.h"
#include "guiWIndow.h"


namespace gui
{
	guiWindow::guiWindow(const std::string_view _strKey)
		: guiBase(_strKey)
		, mWindowFlag()
		//, mSize()
	{
	}
	guiWindow::~guiWindow()
	{
	}
	bool guiWindow::BeginUI()
	{
		bool retVal = ImGui::Begin(GetKey().c_str(), GetEnablePtr(), mWindowFlag);
		if (retVal)
		{
			//if (ImGuiWindowFlags_::ImGuiWindowFlags_NoResize & mWindowFlag)
			//{
			//	ImGui::SetNextWindowSize(mSize);
			//}
		}
		else
		{
			ImGui::End();
		}

		return retVal;
	}
	void guiWindow::EndUI()
	{
		ImGui::End();
	}
}
