#pragma once
#include "guiWidget.h"

#include <functional>

namespace gui
{
	class TreeWidget : public Widget
	{
	public:
		struct tNode : public Entity
		{
			tNode();
			~tNode();

			void Update();

			void SetData(void* data) { mData = data; }
			void SetStem(bool bEnable) { mbStem = bEnable; }

			void AddNode(tNode* node);
			const std::vector<tNode*>& GetChilds() { return mChilds; }

			TreeWidget* mTreeWidget;
			void* mData;

			tNode* mParent;
			std::vector<tNode*> mChilds;

			bool mbStem;
			bool mbSelected;
		};

		TreeWidget();
		virtual ~TreeWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Close() override;

		tNode* AddNode(tNode* parent, const std::string& name, void* data, bool stem = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(tNode* node);

		void SetEvent(Widget* widget, std::function<void(void* data)> func)
		{
			mEventWidget = widget;
			mEvent = func;
		}


	private:
		tNode* mRoot;

		bool mbDummyRootUse;
		tNode* mSelectedNode;

		Widget* mEventWidget;
		std::function<void(void* data)> mEvent;
	};
}
