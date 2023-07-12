#include "guiListWidget.h"

namespace gui
{
	ListWidget::ListWidget()
		: Widget()
		, mListIndex(-1)
	{
		SetName("ListWidget");
		SetState(eState::Paused);
		SetSize(ImVec2(200.0f, 200.0f));
	}

	ListWidget::~ListWidget()
	{
	}

	void ListWidget::Update()
	{

		if (ImGui::BeginListBox("##ListBox", GetSize()))
		{
			WindowFocus(true);
			for (size_t i = 0; i < mResourceList.size(); i++)
			{
				bool bselectable = (mListIndex == i);
				if (ImGui::Selectable(mResourceList[i].c_str(), &bselectable))
					mListIndex = i;

				if (bselectable)
				{
					ImGui::SetItemDefaultFocus();
				}
				
				// 해당 아이템이 더블클릭 되었다면
				if (ImGui::IsItemHovered() 
					&& ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (mEventWidget && mEvent)
					{
						mEvent(mResourceList[i].c_str());
					}
					Close();
				}

			}
			ImGui::EndListBox();
		}
	}

	void ListWidget::Close()
	{
		SetState(eState::Paused);
		mResourceList.clear();

		WindowFocus(false);
	}

	void ListWidget::SetItemList(const std::vector<std::wstring>& list)
	{
		for (auto wName : list)
		{
			std::string name(wName.begin(), wName.end());
			mResourceList.push_back(name);
		}
	}

	void ListWidget::SetItemList(const std::vector<std::string>& list)
	{
		mResourceList = list;
	}
}