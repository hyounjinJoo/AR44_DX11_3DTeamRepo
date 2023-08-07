#include "PCH_Contents.h"
#include "Scene_Play.h"

#include <Engine/Com_Transform.h>
#include <Engine/Com_Renderer_Mesh.h>
#include <Engine/RenderMgr.h>
#include <Engine/ResMgr.h>
#include <Engine/Texture.h>
#include <Engine/Script_Player.h>
#include <Engine/Com_Camera.h>
#include <Engine/Script_Camera.h>
#include <Engine/Com_Renderer_Sprite.h>
#include <Engine/GridScript.h>
#include <Engine/Object.h>
#include <Engine/InputMgr.h>

namespace mh
{
	Scene_Play::Scene_Play()
		: IScene()
	{
		
	}

	Scene_Play::~Scene_Play()
	{
	}

	void Scene_Play::Init()
	{

		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Com_Camera, this);
		Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<Script_Camera>();

		IScene::Init();
	}

	void Scene_Play::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::N))
		{
			//SceneMgr::LoadScene(eSceneType::Tilte);
		}

		IScene::Update();
	}

	void Scene_Play::FixedUpdate()
	{
		IScene::FixedUpdate();
	}

	void Scene_Play::Render()
	{
		IScene::Render();
	}

	void Scene_Play::OnEnter()
	{
	}

	void Scene_Play::OnExit()
	{
	}
}
