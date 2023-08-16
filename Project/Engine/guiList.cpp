#include "PCH_Engine.h"
#include "guiList.h"

#include "define_Util.h"

namespace gui
{
	guiList::guiList()
		: guiBase(strKey::ListBox)
		, mListIndex(-1)
		, mSize(200.f, 200.f)
		, mEventGUI()
	{
		//SetState(eState::Paused);
	}

	guiList::~guiList()
	{
	}

	bool guiList::BeginUI()
	{
		bool retval = ImGui::BeginListBox(GetKey().c_str(), mSize);
		if (retval)
		{
			ImGui::SetNextWindowFocus();
		}
		return retval;
	}

	void guiList::UpdateUI()
	{
		ImGui::SetWindowFocus(GetKey().c_str());
		for (size_t i = 0; i < mResourceList.size(); i++)
		{
			bool bselectable = (mListIndex == i);
			if (ImGui::Selectable(mResourceList[i].c_str(), &bselectable))
			{
				mListIndex = (int)i;
			}

			if (bselectable)
			{
				ImGui::SetItemDefaultFocus();
			}
				
			// 해당 아이템이 더블클릭 되었다면
			if (ImGui::IsItemHovered() 
				&& ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (mEventGUI && mEvent)
				{
					mEvent(mResourceList[i].c_str());
				}
				mResourceList.clear();
				ImGui::SetWindowFocus(nullptr);
				Close();
			}
		}
		
	}

	void guiList::EndUI()
	{
		ImGui::EndListBox();
	}

	void guiList::SetItemList(const std::vector<std::wstring>& _vecItem)
	{
		for (const auto& wName : _vecItem)
		{
			std::string name = StringConv::ConvertUnicodeToUTF8(wName);
			mResourceList.push_back(name);
		}
	}

	void guiList::SetItemList(const std::vector<std::string>& _vecItem)
	{
		mResourceList = _vecItem;
	}
}
