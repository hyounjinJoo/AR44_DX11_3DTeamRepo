#include "EnginePCH.h"

#include "GridScript.h"
#include "Transform.h"
#include "GameObject.h"
#include "Application.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "SceneManager.h"

extern mh::Application application;

namespace mh
{
	GridScript::GridScript()
		: Script()
		, mCamera(nullptr)
	{

	}

	GridScript::~GridScript()
	{

	}

	void GridScript::Initialize()
	{
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		mCamera = renderer::gCameras[(UINT)type][0];
	}

	void GridScript::Update()
	{
		if (mCamera == nullptr)
		{
			return;
		}

		GameObject* gameObj = mCamera->GetOwner();
		Transform* TR = gameObj->GetComponent<Transform>();
		
		Vector3 cameraPosition = TR->GetPosition();
		Vector4 position = Vector4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

		float scale = mCamera->GetScale();

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		Vector2 resolution(width, height);

		// Constant buffer
		graphics::ConstantBuffer* CB = renderer::constantBuffers[(UINT)graphics::eCBType::Grid];
		renderer::GridCB data;
		data.CameraPosition = position;
		data.CameraScale = Vector2(scale, scale);
		data.Resolution = resolution;
		
		CB->SetData(&data);
		CB->Bind(graphics::eShaderStage::VS);
		CB->Bind(graphics::eShaderStage::PS);
 	}

	void GridScript::FixedUpdate()
	{
	}

	void GridScript::Render()
	{
	}

}
