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

	void GridScript::Initalize()
	{
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		mCamera = renderer::cameras[(UINT)type][0];
	}

	void GridScript::Update()
	{
		if (mCamera == nullptr)
		{
			return;
		}

		GameObject* gameObj = mCamera->GetOwner();
		Transform* TR = gameObj->GetComponent<Transform>();
		
		Vector3 cameraPosition = tr->GetPosition();
		Vector4 position = Vector4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

		float scale = mCamera->GetScale();

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		float width = winRect.right - winRect.left;
		float height = winRect.bottom - winRect.top;
		Vector2 resolution(width, height);

		// Constant buffer
		ConstantBuffer* CB = renderer::constantBuffers[(UINT)eCBType::Grid];
		renderer::GridCB data;
		data.cameraPosition = position;
		data.cameraScale = Vector2(scale, scale);
		data.resolution = resolution;

		CB->SetData(&data);
		CB->Bind(eShaderStage::VS);
		CB->Bind(eShaderStage::PS);
 	}

	void GridScript::FixedUpdate()
	{
	}

	void GridScript::Render()
	{
	}

}