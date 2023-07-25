#include "ClientPCH.h"
#include "guiTransform.h"

#include <Engine/Transform.h>


namespace gui
{
	//TODO: 정상작동 안함
	Transform::Transform()
		: IComponent(mh::define::eComponentType::Animator)
	{
		SetKey("Transform");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	Transform::~Transform()
	{

	}

	void Transform::FixedUpdate()
	{
		IComponent::FixedUpdate();


		if (GetTarget() == nullptr)
			return;

		mh::Transform* tr = GetTarget()->GetComponent<mh::Transform>();

		mPosisition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
	}

	void Transform::Update()
	{
		IComponent::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosisition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			mh::Transform* tr = GetTarget()->GetComponent<mh::Transform>();

			tr->SetPosition(mPosisition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}

	void Transform::LateUpdate()
	{
		IComponent::LateUpdate();

	}
}
