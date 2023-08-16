#include "PCH_Engine.h"

#include "guiResource.h"

namespace gui
{
	guiResource::guiResource(mh::eResourceType _resType)
		: guiChild(mh::define::strKey::ArrResName[(int)_resType])
		, mTarget()
	{

	}

	guiResource::~guiResource()
	{

	}

	void guiResource::UpdateUI()
	{
		if (mTarget == nullptr)
			return;

		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		ImGui::Button(mh::define::strKey::ArrResName[(UINT)mTarget->GetResType()]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}
