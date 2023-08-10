#include "PCH_Client.h"

#include "guiComponent.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
	guiComponent::guiComponent(mh::define::eComponentType _type)
		: guiChild("Component")
		, mType(_type)
		, mTarget(nullptr)
	{
	}

	guiComponent::~guiComponent()
	{

	}


	void guiComponent::UpdateUI()
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		ImGui::Button(mh::define::ArrComName[(UINT)mType]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}
