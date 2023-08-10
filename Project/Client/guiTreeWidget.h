#pragma once
#include "guiChild.h"
#include <functional>

namespace gui
{
	struct tData
	{
		void* pData;
		size_t size;

		template <typename T>
		void SetDataPtr(const T _pData) { pData = (void*)_pData; size = sizeof(T); }
	};

	class TreeWidget : public guiChild
	{
	public:
		struct tNode : public guiEntity
		{
			tNode();
			virtual ~tNode();

			void Update();

			void SetData(tData _data) { mData = _data; }

			void SetStem(bool bEnable) { mbStem = bEnable; }

			void AddNode(tNode* node);
			const std::vector<tNode*>& GetChilds() { return mChilds; }

			TreeWidget* mTreeWidget;
			tData mData;

			tNode* mParent;
			std::vector<tNode*> mChilds;

			bool mbStem;
			bool mbSelected;
		};

		TreeWidget();
		virtual ~TreeWidget();

		virtual void UpdateUI() override;

		tNode* AddNode(tNode* parent, const std::string& name, tData data, bool stem = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(tNode* node);

		void SetEvent(guiBase* widget, std::function<void(tData)> func)
		{
			mEventGUI = widget;
			mEvent = func;
		}


	private:
		std::unique_ptr<tNode> mRoot;

		bool mbDummyRootUse;
		tNode* mSelectedNode;

		guiBase* mEventGUI;
		std::function<void(tData data)> mEvent;
	};
}
