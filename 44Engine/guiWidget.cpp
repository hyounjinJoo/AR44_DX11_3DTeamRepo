#include "guiWidget.h"

namespace gui
{
	Widget::Widget()
		: Entity()
		, mState(eState::Active)
		, mParent(nullptr)
		, mChilds{}
		, mWindow_flags(ImGuiWindowFlags_None)
	{

	}

	Widget::~Widget()
	{
	}

	void Widget::FixedUpdate()
	{
		if (mState != eState::Active)
			return;

		//for (Widget* child : mChilds)
		//{
		//	child->FixedUpdate();
		//}
	}

	void Widget::Update()
	{
		if (mState != eState::Active)
			return;

		//for ( Widget* child : mChilds )
		//{
		//	child->Update();
		//}
	}

	void Widget::LateUpdate()
	{
		if (mState != eState::Active)
			return;

		//for (Widget* child : mChilds)
		//{
		//	child->LateUpdate();
		//}
	}

	void Widget::Render()
	{
		if (mState != eState::Active)
			return;

		// 최상위부모 일때
		if (mParent == nullptr) 
		{
			bool open = (bool)GetState();
			FixedUpdate();
			ImGui::Begin(GetName().c_str(), &open, mWindow_flags);
			Update();
			for (Widget* child : mChilds)
			{
				child->Render();
			}
			LateUpdate();

			ImGui::End();
		}
		else
		{
			FixedUpdate();
			ImGui::BeginChild(GetName().c_str(), mSize);
			//size 추가 해줘야한다.-
			Update();
			for (Widget* child : mChilds)
			{
				child->Render();
			}
			LateUpdate();
			//std::string debug = GetName();
			ImGui::EndChild();
		}
	}

	void Widget::Close()
	{
	}

	void Widget::AddWidget(Widget* widget)
	{
		mChilds.push_back(widget);
		widget->SetParent(this);
	}

	void Widget::WindowFocus(bool enable)
	{
		if (enable == true)
			ImGui::SetWindowFocus(GetName().c_str());
		else
			ImGui::SetWindowFocus(nullptr);
	}
}
