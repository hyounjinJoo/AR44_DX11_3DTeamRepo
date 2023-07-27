#include "ClientPCH.h"
#include "guiTransform.h"

#include <Engine/Com_Transform.h>


namespace gui
{
	guiTransform::guiTransform()
		: guiComponent(mh::define::eComponentType::Transform)
	{
		SetKey("guiTransform");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	guiTransform::~guiTransform()
	{

	}

	void guiTransform::FixedUpdate()
	{
		Widget::FixedUpdate();

		if (nullptr == mTarget)
			return;

		mh::Com_Transform* tr = GetTarget()->GetComponent<mh::Com_Transform>();

		mPosisition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
	}

	void guiTransform::Update()
	{
		Widget::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosisition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			mh::Com_Transform* tr = GetTarget()->GetComponent<mh::Com_Transform>();

			tr->SetPosition(mPosisition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}

	void guiTransform::LateUpdate()
	{
		Widget::LateUpdate();

	}
}
