#include "EnginePCH.h"

#include "PlayScene.h"
#include "Transform.h"
#include "Com_Renderer_Mesh.h"
#include "Renderer.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Script_Player.h"
#include "Com_Camera.h"
#include "Script_Camera.h"
#include "Com_Renderer_Sprite.h"
#include "GridScript.h"
#include "Object.h"
#include "Input.h"

namespace mh
{
	PlayScene::PlayScene()
		: Scene(eSceneType::Play)
	{
		
	}

	PlayScene::~PlayScene()
	{
	}

	void PlayScene::Initialize()
	{
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Com_Camera, this);
		Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<Script_Camera>();

		Scene::Initialize();
	}

	void PlayScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Tilte);
		}

		Scene::Update();
	}

	void PlayScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}

	void PlayScene::Render()
	{
		Scene::Render();
	}

	void PlayScene::OnEnter()
	{
	}

	void PlayScene::OnExit()
	{
	}
}
