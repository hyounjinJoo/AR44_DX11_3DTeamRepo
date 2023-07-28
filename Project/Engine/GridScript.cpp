#include "EnginePCH.h"

#include "GridScript.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Application.h"
#include "ConstBuffer.h"
#include "RenderMgr.h"
#include "SceneManager.h"

extern mh::Application application;

namespace mh
{
	GridScript::GridScript()
		: IScript()
		, mCamera(nullptr)
	{

	}

	GridScript::~GridScript()
	{

	}

	void GridScript::Initialize()
	{
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		mCamera = RenderMgr::GetInst()->GetCamera(type, 0);
	}

	void GridScript::Update()
	{
		if (mCamera == nullptr)
		{
			return;
		}

		GameObject* gameObj = mCamera->GetOwner();
		Com_Transform& TR = gameObj->GetTransform();
		
		Vector3 cameraPosition = TR.GetPosition();
		Vector4 position = Vector4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

		float scale = mCamera->GetScale();

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		Vector2 resolution(width, height);

		// Constant buffer
		ConstBuffer* CB = RenderMgr::GetInst()->GetConstBuffer(eCBType::Grid);
		GridCB data;
		data.CameraPosition = position;
		data.CameraScale = Vector2(scale, scale);
		data.Resolution = resolution;
		
		CB->SetData(&data);
		CB->BindData(eShaderStageFlag::VS | eShaderStageFlag::PS);
 	}

	void GridScript::FixedUpdate()
	{
	}

	void GridScript::Render()
	{
	}

}
