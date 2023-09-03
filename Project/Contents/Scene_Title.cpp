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
#include <Engine/Com_Animator3D.h>
#include <Engine/PaintShader.h>
#include <Engine/Com_Renderer_ParticleSystem.h>
#include <Engine/Prefab.h>
#include <Engine/Com_Renderer_UIBase.h>
#include <Contents/Script_UIBase.h>
#include <Contents/Script_UIGauge.h>

#include "strKey_Script.h"
#include "strKey_Component.h"


#include <Engine/EventMgr.h>

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
			GameObject* cameraObj = EventMgr::SpawnGameObject(eLayerType::Com_Camera);
			cameraObj->SetName("MainCamera");

			Com_Transform* tr = cameraObj->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(0.0f, 0.0f, -20.0f));

			Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
			cameraComp->SetProjectionType(define::eProjectionType::Perspective);

			cameraObj->AddComponent(strKey::Script::Script_CameraMove);
			cameraObj->AddComponent(strKey::Script::Script_UIBase);


			RenderMgr::SetMainCamera(cameraComp);
		}

		{
			GameObject* dirLight = EventMgr::SpawnGameObject(eLayerType::Player);
			dirLight->AddComponent<Com_Transform>();

			Com_Light3D* light3d = dirLight->AddComponent<Com_Light3D>();
			light3d->SetLightType(eLightType::Directional);
			light3d->SetDiffuse(float4(0.3f, 0.3f, 0.3f, 1.f));
			light3d->SetAmbient(float4(0.3f, 0.3f, 0.3f, 1.f));
		}




		{
			std::shared_ptr<MeshData> meshdata = ResMgr::Load<MeshData>("nergigante");

			GameObject* modeling = meshdata->Instantiate();

			modeling->GetComponent<Com_Animator3D>()->Play("NlaTrack");
			
			EventMgr::SpawnGameObject(define::eLayerType::Player, modeling);
		}




		{
			// Main Com_Camera Game Object
			GameObject* cameraObj = EventMgr::SpawnGameObject(eLayerType::Com_Camera);
			cameraObj->SetName("UICam");

			Com_Transform* tr = cameraObj->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(0.0f, 0.0f, -20.0f));

			Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
			cameraComp->SetProjectionType(define::eProjectionType::Orthographic);

			//다른 레이어는 전부 끈다음
			cameraComp->DisableLayerMasks();

			//UI 레이어만 촬영하도록 설정한다.
			cameraComp->TurnLayerMask(eLayerType::UI, true);
		}


		{
			GameObject* baseUI = EventMgr::SpawnGameObject(eLayerType::UI);
			baseUI->SetName("BaseUI");
			Com_Transform* tr = baseUI->AddComponent<Com_Transform>();
			tr->SetSize(float3(100.f, 100.f, 1.f));

			//UI 스크립트 추가
			//Base를 추가하게 되면 단순 이미지 한 장을 출력하는 UI가 생성됨
			Script_UIBase* ui = baseUI->AddComponent<Script_UIBase>();

			std::shared_ptr<Texture> smileTex = ResMgr::Load<Texture>("Smile.png");
			ui->SetTexture(define::eTextureSlot::Tex_0, smileTex);
		}


		{
			GameObject* GaugeUI = EventMgr::SpawnGameObject(eLayerType::UI);
			GaugeUI->SetName("GaugeUI");
			Com_Transform* tr = GaugeUI->AddComponent<Com_Transform>();
			tr->SetSize(float3(100.f, 10.f, 1.f));
			tr->SetRelativePosXY(float2(100.f, 100.f));


			//게이지 UI 생성(마찬가지로 스크립트만 추가하면 Gauge UI 모드로 세팅이 됨)
			Script_UIGauge* gauge = GaugeUI->AddComponent<Script_UIGauge>();


			gauge->SetGaugeRemain(0.5f);
			std::shared_ptr<Texture> magenta = ResMgr::Load<Texture>("Durability_3.png");
			gauge->SetTexture(define::eTextureSlot::Tex_0, magenta);
		}

		//게이지 UI는 단순하게 쉐이더를 교체해주는 것만으로도 설정이 가능하므로 BaseUI 컴포넌트를 쓰고, 쉐이더만 교체
		{
			GameObject* customGauge = EventMgr::SpawnGameObject(eLayerType::UI);
			customGauge->SetName("GaugeUI");
			Com_Transform* tr = customGauge->AddComponent<Com_Transform>();

			//게이지 UI 생성(마찬가지로 스크립트만 추가하면 Gauge UI 모드로 세팅이 됨)
			Script_UIGauge* gauge = customGauge->AddComponent<Script_UIGauge>();
			gauge->SetGaugeRemain(0.5f);

			//커스텀한 게이지 재질을 직접 수동으로 설정해줄수도 있음
			//커스텀 픽셀 쉐이더 + 알파 블렌딩
			std::shared_ptr<Material> mtrl = ResMgr::Load<Material>("UI/CustomGauge.json");
			gauge->SetCustomMaterial(mtrl);

			tr->SetSize(float3(100.f, 30.f, 1.f));
			tr->SetRelativePosXY(float2(200.f, 200.f));
		}


		

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
