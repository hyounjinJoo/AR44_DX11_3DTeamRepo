#pragma once
#include "guiWidget.h"

namespace gui
{
	class TreeWidget : public Widget
	{
	public:
		struct Node : public Entity
		{
			Node();
			~Node();

			void Update();

			void SetData(void* data) { mData = data; }
			void SetStem(bool enable) { mbStem = enable; }

			void AddNode(Node* node);
			const std::vector<Node*>& GetChilds() { return mChilds; }

			TreeWidget* mTreeWidget;
			void* mData;

			Node* mParent;
			std::vector<Node*> mChilds;

			bool mbStem;
			bool mbSelected;
		};

		TreeWidget();
		~TreeWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Close() override;

		Node* AddNode(Node* parent, const std::string& name, void* data, bool stem = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(Node* node);

		void SetEvent(Widget* widget, std::function<void(void* data)> func)
		{
			mEventWidget = widget;
			mEvent = func;
		}


	private:
		Node* mRoot;

		//
		bool mbDummyRootUse;
		Node* mSelectedNode;

		Widget* mEventWidget;
		std::function<void(void* data)> mEvent;
	};
}