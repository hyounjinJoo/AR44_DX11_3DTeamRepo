#include "PCH_Client.h"
#include "guiTransform.h"

#include <Engine/Com_Transform.h>


namespace gui
{
	guiTransform::guiTransform()
		: guiComponent(mh::define::eComponentType::Transform)
		, mType()
		, mTarget()
	{
		SetKey("guiTransform");
		//SetSize(ImVec2(200.0f, 120.0f));
	}

	guiTransform::~guiTransform()
	{

	}

	void guiTransform::Update()
	{
		if (nullptr == mTarget)
			return;

		mh::Com_Transform* tr = GetTarget()->GetComponent<mh::Com_Transform>();

		mPosisition = tr->GetRelativePos();
		mRotation = tr->GetRelativeRotXYZ();
		mScale = tr->GetRelativeScale();
	}

	void guiTransform::UpdateUI()
	{
		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosisition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			mh::Com_Transform* tr = GetTarget()->GetComponent<mh::Com_Transform>();

			tr->SetRelativePos(mPosisition);
			tr->SetRelativeRotXYZ(mRotation);
			tr->SetRelativeScale(mScale);
		}
	}

}
