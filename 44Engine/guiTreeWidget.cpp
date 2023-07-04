#include "guiTreeWidget.h"


namespace gui
{
	TreeWidget::Node::Node()
		: mTreeWidget(nullptr)
		, mData(nullptr)
		, mParent(nullptr)
		, mbSelected(false)
		, mbStem(false)
	{
	}
	TreeWidget::Node::~Node()
	{
		for (Node* child : mChilds)
		{
			delete child;
			child = nullptr;
		}
	}
	// Node
	void TreeWidget::Node::Update()
	{
		//ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;

		int flag = 0;

		if (mbStem)
			flag |= ImGuiTreeNodeFlags_Framed;
		if (mbSelected)
			flag |= ImGuiTreeNodeFlags_Selected;
		if (mChilds.empty())
			flag |= ImGuiTreeNodeFlags_Leaf;

		if (mbStem && mChilds.empty())
			SetName("\t" + GetName());

		if (ImGui::TreeNodeEx(GetName().c_str(), flag))
		{
			if (!mbStem && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				mTreeWidget->SetSelectedNode(this);
			}

			for (Node* node : mChilds)
				node->Update();

			ImGui::TreePop();
		}
	}

	void TreeWidget::Node::AddNode(Node* node)
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
			return;

		//mRoot->Update();

		if (!mbDummyRootUse)
		{
			mRoot->Update();
		}
		else
		{
			const std::vector<Node*>& childs = mRoot->GetChilds();
			for (Node* child : childs)
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

	TreeWidget::Node* TreeWidget::AddNode(Node* parent, const std::string& name, void* data, bool isFrame)
	{
		Node* node = new Node;
		node->SetName(name);
		node->SetData(data);
		node->SetStem(isFrame);
		node->mTreeWidget = this;

		if (nullptr == parent)
			mRoot = node;
		else
			parent->AddNode(node);

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
	void TreeWidget::SetSelectedNode(Node* node)
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
