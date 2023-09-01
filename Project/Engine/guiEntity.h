#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include <string>
#include "json-cpp/json-forwards.h"
#include "define_Enum.h"


namespace gui
{
	class guiEntity
	{
	public:
		guiEntity(const std::string_view _strName);
		virtual ~guiEntity();

		virtual mh::eResult SaveJson(Json::Value* _pJval) { return mh::eResult::Success; }
		virtual mh::eResult LoadJson(const Json::Value* _pJval) { return mh::eResult::Success; };

		UINT32 GetID() const { return mID; }

		const std::string& GetName() const { return mName; }

		void MakeUniqueKeyByName() { mStrKey += "##"; mStrKey += std::to_string(mID); }
		void SetKey(const std::string_view _strKey) { mStrKey = _strKey; }
		const std::string& GetKey() const { return mStrKey; }
		
		void SetSaveEnable(bool _bSaveEnable) { mbSaveEnable = _bSaveEnable; }
		bool IsSaveEnable() const { return mbSaveEnable; }

	private:
		static UINT32				gIDNext;
		const UINT32				mID;

		//이름(중복 가능)
		std::string					mName;
		
		//Key(중복 불가)
		std::string					mStrKey;

		//데이터를 저장할지 말지 결정(기본 Off)
		bool						mbSaveEnable;
	};

	inline void HilightText(const char* _label, const ImVec2& _size = ImVec2(0.f, 0.f))
	{
		ImGui::PushID(0);
		static ImColor highLight = { 144, 12, 63 };
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)highLight);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)highLight);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)highLight);

		ImGui::Button(_label, _size);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}


