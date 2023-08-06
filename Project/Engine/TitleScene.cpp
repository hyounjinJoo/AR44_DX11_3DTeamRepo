#include "PCH_Engine.h"

#include "ResMgr.h"



#include "TitleScene.h"
#include "Com_Transform.h"
#include "Com_Renderer_Mesh.h"
#include "RenderMgr.h"
#include "Texture.h"
#include "Script_Player.h"
#include "Com_Camera.h"
#include "Script_Camera.h"
#include "Com_Renderer_Sprite.h"
#include "GridScript.h"
#include "Object.h"
#include "InputMgr.h"
#include "ICollider2D.h"
#include "Player.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "Com_Animator.h"
#include "Com_Light.h"
#include "PaintShader.h"
#include "Com_Renderer_ParticleSystem.h"
#include "Prefab.h"

namespace mh
{
	TitleScene::TitleScene()
		: Scene(eSceneType::Tilte)
	{
	}
	TitleScene::~TitleScene()
	{
	}
	void TitleScene::Init()
	{
		// Main Com_Camera Game Object
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Com_Camera);
		cameraObj->SetName("MainCamera");
		cameraObj->GetTransform().SetPosition(float3(0.0f, 0.0f, -20.0f));
		Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
		cameraComp->SetProjectionType(define::eProjectionType::Perspective);
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<Script_Camera>();

		RenderMgr::SetMainCamera(cameraComp);

		//포워드 렌더링으로 그려지는 오브젝트
		{
			GameObject* player = object::Instantiate<GameObject>(eLayerType::Player);
			player->GetTransform().SetPosition(float3(0.0f, 0.0f, 10.0f));
			player->GetTransform().SetScale(float3(5.0f, 5.0f, 5.0f));
			//player->GetTransform().SetRotation(float3(15.0f, 45.0f, 0.0f));
			player->SetName("Player");
			Com_Renderer_Mesh* mr = player->AddComponent<Com_Renderer_Mesh>();
			mr->SetMaterial(ResMgr::Find<Material>(strKey::Default::material::Basic3DMaterial));
			mr->SetMesh(ResMgr::Find<Mesh>(strKey::Default::mesh::CubeMesh));
			player->AddComponent<Script_Player>();

			auto* AudioSource = player->AddComponent<Com_AudioSource>();
			
			auto TestClip = ResMgr::Load<AudioClip>("Test.mp3");
			AudioSource->SetClip(TestClip);

			auto* Animator = player->AddComponent<Com_Animator>();

			Prefab forSave{};
			forSave.RegisterPrefab(player, true);
			forSave.Save("TestPrefab.json");
		}

		//디퍼드 렌더링으로 그려지는 오브젝트
		{
			GameObject* player = object::Instantiate<GameObject>(eLayerType::Player);
			player->GetComponent<Com_Transform>()->SetPosition(float3(-15.0f, 0.0f, 10.0f));
			player->GetComponent<Com_Transform>()->SetScale(float3(5.0f, 5.0f, 5.0f));
			//player->GetComponent<Transform>()->SetRotation(float3(15.0f, 45.0f, 0.0f));
			player->SetName("Player");
			Com_Renderer_Mesh* mr = player->AddComponent<Com_Renderer_Mesh>();
			mr->SetMaterial(ResMgr::Find<Material>(strKey::Default::material::DefferedMaterial));
			mr->SetMesh(ResMgr::Find<Mesh>(strKey::Default::mesh::CubeMesh));
			player->AddComponent<Script_Player>();
		}


		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName("directionalLight");

			directionalLight->GetComponent<Com_Transform>()->SetPosition(float3(0.0f, 100.0f, 0.0f));
			directionalLight->GetComponent<Com_Transform>()->SetRotation(float3(45.0f, 0.0f, 0.0f));

			Com_Light* lightComp = directionalLight->AddComponent<Com_Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(float4(1.0f, 0.0f, 0.0f, 1.0f));
			lightComp->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(float4(0.15f, 0.15f, 0.15f, 1.0f));
		}


		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName("PointLight1");

			directionalLight->GetComponent<Com_Transform>()->SetPosition(float3(0.0f, 0.0f, 0.0f));

			Com_Light* lightComp = directionalLight->AddComponent<Com_Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetRadius(20.0f);
			lightComp->SetDiffuse(float4(0.0f, 0.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(float4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName("PointLight2");

			directionalLight->GetComponent<Com_Transform>()->SetPosition(float3(-15.0f, 0.0f, 0.0f));

			Com_Light* lightComp = directionalLight->AddComponent<Com_Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetRadius(30.0f);
			lightComp->SetDiffuse(float4(0.0f, 1.0f, 0.0f, 1.0f));
			lightComp->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(float4(0.15f, 0.15f, 0.15f, 1.0f));
		}


		Scene::Init();
	}
	void TitleScene::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Play);
		}

		Scene::Update();
	}
	void TitleScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}
	void TitleScene::Render()
	{
		Scene::Render();
	}
	void TitleScene::OnEnter()
	{
	}
	void TitleScene::OnExit()
	{
	}
}
