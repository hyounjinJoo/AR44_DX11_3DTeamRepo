#include "PCH_Contents.h"
#include "Scene_Stage01.h"

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
#include <Engine/EventMgr.h>

namespace mh
{
	Scene_Stage01::Scene_Stage01()
		: IScene()
	{

	}

	Scene_Stage01::~Scene_Stage01()
	{
	}

	void Scene_Stage01::Init()
	{
		GameObject* cameraObj = EventMgr::SpawnGameObject(new GameObject, eLayerType::Com_Camera);
		//GameObject* cameraObj = object::Instantiate(eLayerType::Com_Camera, new GameObject);
		Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<Script_Camera>();

		IScene::Init();
	}

	void Scene_Stage01::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::N))
		{
			//SceneMgr::LoadScene(eSceneType::Tilte);
		}

		IScene::Update();
	}

	void Scene_Stage01::FixedUpdate()
	{
		IScene::FixedUpdate();
	}

	void Scene_Stage01::Render()
	{
		IScene::Render();
	}

	void Scene_Stage01::OnEnter()
	{
	}

	void Scene_Stage01::OnExit()
	{
	}
}
