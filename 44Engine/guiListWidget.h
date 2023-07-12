#pragma once
#include "guiWidget.h"

namespace gui
{
	class ListWidget : public Widget
	{
	public:
		ListWidget();
		virtual ~ListWidget();

		virtual void Update() override;
		void Close() override;

		void SetEvent(Widget* widget, std::function<void(std::string key)> event)
		{
			mEventWidget = widget;
			mEvent = std::move(event);
		}

		void SetItemList(const std::vector<std::wstring>& list);
		void SetItemList(const std::vector<std::string>& list);

	private:
		std::vector<std::string> mResourceList;
		int mListIndex;

		Widget* mEventWidget;
		std::function<void(std::string key)> mEvent;
	};
}
