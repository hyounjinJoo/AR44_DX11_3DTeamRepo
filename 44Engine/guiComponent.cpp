#include "guiComponent.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
	Component::Component(ya::enums::eComponentType type)
		: mType(type)
		, mTarget(nullptr)
	{


	}

	Component::~Component()
	{

	}

	void Component::FixedUpdate()
	{

	}

	void Component::Update()
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		ImGui::Button(ya::enums::charComponentType[(UINT)mType]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	void Component::LateUpdate()
	{

	}
}
