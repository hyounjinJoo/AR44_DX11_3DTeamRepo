#include "PCH_Client.h"

#include "guiTreeWidget.h"

namespace gui
{
	TreeWidget::tNode::tNode()
		: mTreeWidget(nullptr)
		, mData(nullptr)
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
	void TreeWidget::tNode::Update()
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
				node->Update();
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
		: mRoot(nullptr)
	{

	}

	TreeWidget::~TreeWidget()
	{
		delete mRoot;
		mRoot = nullptr;
	}

	void TreeWidget::FixedUpdate()
	{
	}

	void TreeWidget::Update()
	{
		if (mRoot == nullptr)
		{
			return;
		}

		//mRoot->Update();

		if (!mbDummyRootUse)
		{
			mRoot->Update();
		}
		else
		{
			const std::vector<tNode*>& childs = mRoot->GetChilds();
			for (tNode* child : childs)
			{
				child->Update();
			}
		}
	}

	void TreeWidget::LateUpdate()
	{
	}

	void TreeWidget::Close()
	{
	}

	TreeWidget::tNode* TreeWidget::AddNode(tNode* parent, const std::string& name, void* data, bool isFrame)
	{
		tNode* node = new tNode;
		node->SetKey(name);
		node->SetData(data);
		node->SetStem(isFrame);
		node->mTreeWidget = this;

		if (nullptr == parent)
		{
			mRoot = node;
		}
		else
		{
			parent->AddNode(node);
		}

		return node;
	}

	void TreeWidget::Clear()
	{
		if (nullptr != mRoot)
		{
			delete mRoot;
			mRoot = nullptr;
		}
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
