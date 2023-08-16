#pragma once
#include "guiBase.h"
#include <functional>

namespace gui
{
	class guiBase;
	class guiList : public guiBase
	{
	public:
		guiList();
		virtual ~guiList();

		virtual bool BeginUI() final;
		virtual void UpdateUI() override;
		virtual void EndUI() final;

		void SetEvent(guiBase* widget, std::function<void(const std::string& key)> event)
		{
			mEventGUI = widget;
			mEvent = std::move(event);
		}

		void SetItemList(const std::vector<std::wstring>& _vecItem);
		void SetItemList(const std::vector<std::string>& _vecItem);

	private:
		std::vector<std::string> mResourceList;
		int mListIndex;

		ImVec2 mSize;

		guiBase* mEventGUI;
		std::function<void(std::string key)> mEvent;
	};
}
