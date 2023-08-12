#include "PCH_Engine.h"
#include "guiTreeWidget.h"

namespace gui
{
	TreeWidget::tNode::tNode()
		: guiEntity("")
		, mTreeWidget(nullptr)
		, mData{}
		, mParent(nullptr)
		, mbSelected(false)
		, mbStem(false)
	{
	}
	TreeWidget::tNode::~tNode()
	{
		for (tNode* child : mChilds)
		{
			delete child;
			child = nullptr;
		}
	}
	// Node
	void TreeWidget::tNode::UpdateUI()
	{
		//ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;

		int flag = 0;

		if (mbStem)
		{
			flag |= ImGuiTreeNodeFlags_Framed;
		}
		if (mbSelected)
		{
			flag |= ImGuiTreeNodeFlags_Selected;
		}
		if (mChilds.empty())
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
		}
		if (mbStem && mChilds.empty())
		{
			SetKey("\t" + GetKey());
		}
		if (ImGui::TreeNodeEx(GetKey().c_str(), flag))
		{
			if (!mbStem && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				mTreeWidget->SetSelectedNode(this);
			}

			for (tNode* node : mChilds)
			{
				node->UpdateUI();
			}

			ImGui::TreePop();
		}
	}

	void TreeWidget::tNode::AddNode(tNode* node)
	{
		node->mParent = this;
		mChilds.push_back(node);
	}

	// Tree
	TreeWidget::TreeWidget()
		: guiChild(strKey::TreeWidget)
		, mRoot(nullptr)
		, mbDummyRootUse{}
		, mSelectedNode{}
		, mEventGUI{}
		, mEvent{}
	{
	}

	TreeWidget::~TreeWidget()
	{
	}


	void TreeWidget::UpdateUI()
	{
		if (mRoot == nullptr)
		{
			return;
		}

		if (!mbDummyRootUse)
		{
			mRoot->UpdateUI();
		}
		else
		{
			const std::vector<tNode*>& childs = mRoot->GetChilds();
			for (tNode* child : childs)
			{
				child->UpdateUI();
			}
		}
	}

	TreeWidget::tNode* TreeWidget::AddNode(tNode* parent, const std::string& name, mh::define::tDataPtr data, bool isFrame)
	{
		tNode* node = new tNode;
		node->SetKey(name);
		node->SetData(data);
		node->SetStem(isFrame);
		node->mTreeWidget = this;

		if (nullptr == parent)
		{
			mRoot = std::unique_ptr<tNode>(node);
		}
		else
		{
			parent->AddNode(node);
		}

		return node;
	}

	void TreeWidget::Clear()
	{
		mRoot.reset();
	}
	void TreeWidget::SetSelectedNode(tNode* node)
	{
		if (nullptr != mSelectedNode)
		{
			mSelectedNode->mbSelected = false;
		}

		mSelectedNode = node;
		mSelectedNode->mbSelected = true;

		if (mSelectedNode && mEvent)
		{
			mEvent(mSelectedNode->mData);
		}
	}
}
