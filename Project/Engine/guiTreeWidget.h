#pragma once
#include "guiChild.h"
#include <functional>
#include "define_Struct.h"

namespace gui
{
	class TreeWidget : public guiChild
	{
	public:
		struct tNode : public guiEntity
		{
			tNode();
			virtual ~tNode();

			void UpdateUI();

			void SetData(mh::define::tDataPtr _data) { mData = _data; }

			void SetStem(bool bEnable) { mbStem = bEnable; }

			void AddNode(tNode* node);
			const std::vector<tNode*>& GetChilds() { return mChilds; }

			TreeWidget* mTreeWidget;
			mh::define::tDataPtr mData;

			tNode* mParent;
			std::vector<tNode*> mChilds;

			bool mbStem;
			bool mbSelected;
		};

		TreeWidget();
		virtual ~TreeWidget();

		virtual void UpdateUI() override;

		tNode* AddNode(tNode* parent, const std::string& name, mh::define::tDataPtr data, bool stem = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(tNode* node);

		void SetEvent(guiBase* widget, std::function<void(mh::define::tDataPtr)> func)
		{
			mEventGUI = widget;
			mEvent = func;
		}


	private:
		std::unique_ptr<tNode> mRoot;

		bool mbDummyRootUse;
		tNode* mSelectedNode;

		guiBase* mEventGUI;
		std::function<void(mh::define::tDataPtr data)> mEvent;
	};
}
