#include "PCH_Engine.h"

#include "guiTexture.h"
#include <Engine/Texture.h>

namespace gui
{
	guiTexture::guiTexture()
		: guiResource(mh::define::eResourceType::Texture)
	{

	}

	guiTexture::~guiTexture()
	{

	}

	void guiTexture::UpdateUI()
	{
		mh::Texture* targetTex 
			= dynamic_cast<mh::Texture*>(GetTarget());

		if (targetTex == nullptr)
			return;

		ImGui::Image(targetTex->GetSRV().Get(), ImVec2(150.0f, 150.0f));

		std::string key = targetTex->GetKey();

		ImGui::Text("Key	"); ImGui::SameLine();
		ImGui::InputText("##TexKey"
			, (char*)key.data(), key.size(), ImGuiInputTextFlags_ReadOnly);

		//ImGui::Text("Path	"); ImGui::SameLine();
		//ImGui::InputText("##TexPath"
		//	, (char*)path.data(), path.size(), ImGuiInputTextFlags_ReadOnly);

		int width = (int)targetTex->GetWidth();
		int height = (int)targetTex->GetHeight();


		char wBuffer[256] = "";
		_itoa_s(width, wBuffer, 10);

		ImGui::Text("Width	"); ImGui::SameLine();
		ImGui::InputText("##TexWidth"
			, wBuffer, 256, ImGuiInputTextFlags_ReadOnly);

		char hBuffer[256] = "";
		_itoa_s(height, hBuffer, 10);

		ImGui::Text("Height	"); ImGui::SameLine();
		ImGui::InputText("##TexHeight"
			, hBuffer, 256, ImGuiInputTextFlags_ReadOnly);
	}
}
