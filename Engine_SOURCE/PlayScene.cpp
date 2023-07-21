#include "EnginePCH.h"

#include "PlayScene.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Renderer.h"
#include "GameResources.h"
#include "Texture.h"
#include "PlayerScript.h"
#include "Camera.h"
#include "CameraScript.h"
#include "SpriteRenderer.h"
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
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();

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
