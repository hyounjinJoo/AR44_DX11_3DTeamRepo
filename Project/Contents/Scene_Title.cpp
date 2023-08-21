#include "PCH_Contents.h"
#include "Scene_Title.h"

#include <Engine/ResMgr.h>
#include <Engine/Com_Transform.h>
#include <Engine/Com_Renderer_Mesh.h>
#include <Engine/RenderMgr.h>
#include <Engine/Texture.h>
#include <Engine/Script_Player.h>
#include <Engine/Com_Camera.h>
#include <Engine/Script_Camera.h>
#include <Engine/Com_Renderer_Sprite.h>
#include <Engine/GridScript.h>
#include <Engine/Object.h>
#include <Engine/InputMgr.h>
#include <Engine/ICollider2D.h>
#include <Engine/Player.h>
#include <Engine/Monster.h>
#include <Engine/CollisionMgr.h>
#include <Engine/Com_Animator2D.h>
#include <Engine/Com_Light3D.h>
#include <Engine/PaintShader.h>
#include <Engine/Com_Renderer_ParticleSystem.h>
#include <Engine/Prefab.h>

#include "strKey_Script.h"

#include "Script_TestHJ.h"
#include "Script_JH.h"

namespace mh
{
	Scene_Title::Scene_Title()
	{
	}
	Scene_Title::~Scene_Title()
	{
	}
	void Scene_Title::Init()
	{
		IScene::Init();



		{
			// Main Com_Camera Game Object
			GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Com_Camera);
			cameraObj->SetName("MainCamera");

			Com_Transform* tr = cameraObj->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(0.0f, 0.0f, -20.0f));

			Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
			cameraComp->SetProjectionType(define::eProjectionType::Perspective);
			//cameraComp->RegisterCameraInRenderer();
			//cameraComp->TurnLayerMask(eLayerType::UI, false);
			cameraObj->AddComponent(strKey::Script::Script_CameraMove);
			//cameraObj->AddComponent()

			RenderMgr::SetMainCamera(cameraComp);
		}







		//포워드 렌더링으로 그려지는 오브젝트
		//{
		//	GameObject* player = object::Instantiate<GameObject>(eLayerType::Player);
		//	player->GetComponent<Com_Transform>()->SetRelativePos(float3(0.0f, 0.0f, 10.0f));
		//	player->GetComponent<Com_Transform>()->SetScale(float3(5.0f, 5.0f, 5.0f));
		//	//player->GetComponent<Com_Transform>()->SetRelativeRotXYZ(float3(15.0f, 45.0f, 0.0f));
		//	player->SetName("Player");
		//	Com_Renderer_Mesh* mr = player->AddComponent<Com_Renderer_Mesh>();
		//	mr->SetMaterial(ResMgr::Find<Material>(strKey::Default::material::Basic3DMaterial), 0);
		//	mr->SetMesh(ResMgr::Find<Mesh>(strKey::Default::mesh::CubeMesh));
		//	player->AddComponent<Script_Player>();

		//	auto* AudioSource = player->AddComponent<Com_AudioSource>();
		//	
		//	auto TestClip = ResMgr::Load<AudioClip>("Test.mp3");
		//	AudioSource->SetClip(TestClip);

		//	auto* Animator = player->AddComponent<Com_Animator>();

		//	//Prefab forSave{};
		//	//forSave.RegisterPrefab(player, true);
		//	//forSave.Save("TestPrefab.json");
		//}

		//디퍼드 렌더링으로 그려지는 오브젝트
		//{
		//	GameObject* player = object::Instantiate<GameObject>(eLayerType::Player);
		//	player->GetComponent<Com_Transform>()->SetRelativePos(float3(-15.0f, 0.0f, 10.0f));
		//	player->GetComponent<Com_Transform>()->SetScale(float3(5.0f, 5.0f, 5.0f));
		//	//player->GetComponent<Transform>()->SetRelativeRotXYZ(float3(15.0f, 45.0f, 0.0f));
		//	player->SetName("Player");
		//	Com_Renderer_Mesh* mr = player->AddComponent<Com_Renderer_Mesh>();
		//	mr->SetMaterial(ResMgr::Find<Material>(strKey::Default::material::DefferedMaterial), 0);
		//	mr->SetMesh(ResMgr::Find<Mesh>(strKey::Default::mesh::CubeMesh));
		//	player->AddComponent<Script_Player>();
		//}

		{
			std::shared_ptr<MeshData> data = ResMgr::Load<MeshData>("stm520_038_00.fbx");
			GameObject* obj = data->Instantiate();
			Com_Transform* tr = obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.5f));
			obj->SetName("fbxTextObj");
			obj->AddComponent<Script_Player>();
			object::Instantiate(eLayerType::Player, obj);

			obj->AddComponent<Script_TestHJ>();
			obj->AddComponent<Script_JH>();
		}


		//{
		//	GameObject* player = object::Instantiate<GameObject>(eLayerType::Player);
		//	player->GetComponent<Com_Transform>()->SetRelativePos(float3(-15.0f, 0.0f, 10.0f));
		//	player->GetComponent<Com_Transform>()->SetScale(float3(5.0f, 5.0f, 5.0f));
		//	//player->GetComponent<Transform>()->SetRelativeRotXYZ(float3(15.0f, 45.0f, 0.0f));
		//	player->SetName("Player");
		//	Com_Renderer_Mesh* mr = player->AddComponent<Com_Renderer_Mesh>();
		//	mr->SetMaterial(ResMgr::Find<Material>(strKey::Default::material::DefferedMaterial), 0);
		//	mr->SetMesh(ResMgr::Find<Mesh>(strKey::Default::mesh::CubeMesh));
		//	player->AddComponent<Script_Player>();
		//}


		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName("directionalLight");

			Com_Transform* tr = directionalLight->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(500.f, 500.f, 0.0f));
			tr->SetRelativeRotXYZ(float3(45.0f, 0.0f, 0.0f));

			Com_Light3D* lightComp = directionalLight->AddComponent<Com_Light3D>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(float4(0.7f, 0.7f, 0.7f, 1.0f));
			lightComp->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(float4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		//{
		//	GameObject* pointLight = object::Instantiate<GameObject>(eLayerType::Player);
		//	pointLight->SetName("PointLight1");

		//	Com_Transform* tr = pointLight->AddComponent<Com_Transform>();
		//	tr->SetRelativePos(float3(500.f, 500.f, 0.0f));

		//	Com_Light3D* lightComp = pointLight->AddComponent<Com_Light3D>();
		//	lightComp->SetType(eLightType::Point);
		//	lightComp->SetRadius(20.0f);
		//	lightComp->SetDiffuse(float4(0.0f, 0.0f, 1.0f, 1.0f));
		//	lightComp->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
		//	lightComp->SetAmbient(float4(0.15f, 0.15f, 0.15f, 1.0f));
		//}

		//{
		//	GameObject* pointLight = object::Instantiate<GameObject>(eLayerType::Player);
		//	pointLight->SetName("PointLight2");

		//	Com_Transform* tr = pointLight->AddComponent<Com_Transform>();
		//	tr->SetRelativePos(float3(-500.f, -500.f, 0.0f));

		//	Com_Light3D* lightComp = pointLight->AddComponent<Com_Light3D>();
		//	lightComp->SetType(eLightType::Point);
		//	lightComp->SetRadius(30.0f);
		//	lightComp->SetDiffuse(float4(0.0f, 1.0f, 0.0f, 1.0f));
		//	lightComp->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
		//	lightComp->SetAmbient(float4(0.15f, 0.15f, 0.15f, 1.0f));
		//}
	}
	void Scene_Title::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::N))
		{
			//SceneMgr::LoadScene(eSceneType::Play);
		}

		IScene::Update();
	}
	void Scene_Title::FixedUpdate()
	{
		IScene::FixedUpdate();
	}
	void Scene_Title::Render()
	{
		IScene::Render();
	}
	void Scene_Title::OnEnter()
	{

	}
	void Scene_Title::OnExit()
	{
	}
}
