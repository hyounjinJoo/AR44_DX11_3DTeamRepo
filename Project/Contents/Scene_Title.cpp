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

#include "strKey_Script.h"

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
			GameObject* cameraObj = EventMgr::SpawnGameObject(new GameObject, eLayerType::Com_Camera);
			cameraObj->SetName("MainCamera");

			Com_Transform* tr = cameraObj->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(-4554.0f, 3565.0f, 1097.0f));
			tr->SetRelativeRotXYZ(0.7f, -1.6f, 0.0f);

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
			//std::shared_ptr<MeshData> data = ResMgr::Load<MeshData>("House.fbx");
			//GameObject* obj = data->Instantiate();
			//Com_Transform* tr = obj->GetComponent<Com_Transform>();
			//tr->SetRelativeScale(float3(0.5f));
			//obj->SetName("fbxTextObj");
			//obj->AddComponent<Script_Player>();
			//EventMgr::SpawnGameObject(obj, eLayerType::Player);

			//Com_Animator3D* animator = obj->GetComponent<Com_Animator3D>();
			//if(animator)
			//	animator->Play("NlaTrack.010");

			//
			//object::Instantiate(eLayerType::Player, obj);
			//obj->AddComponent<Script_JH>();
		}

		{
	/*		std::shared_ptr<MeshData> data = ResMgr::Load<MeshData>("DebugMap.json");
			GameObject* obj = data->Instantiate();
			Com_Transform* tr = obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.0f);
			obj->SetName("MainStage");
			obj->AddComponent<Script_Player>();
			EventMgr::SpawnGameObject(obj, eLayerType::Stage);*/
		}


		{
			std::shared_ptr<MeshData> crystalGround2Data = ResMgr::Load<MeshData>("CrystalGround2(stm520_038_00).json");
			GameObject* crystalGround2Obj = crystalGround2Data->Instantiate();
			Com_Transform* tr = crystalGround2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.0f);
			crystalGround2Obj->SetName("CrystalGround2");
			EventMgr::SpawnGameObject(crystalGround2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> bridgeData = ResMgr::Load<MeshData>("stm502_033_00.json");
			GameObject* bridgeObj = bridgeData->Instantiate();
			Com_Transform* tr = bridgeObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-768.0f, 135.0f, 466.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.5f, 3.1f);
			bridgeObj->SetName("Bridge");
			EventMgr::SpawnGameObject(bridgeObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> spreadCrystal4Data = ResMgr::Load<MeshData>("SpreadCrystal4(stm520_031_00).json");
			GameObject* spreadCrystal4Obj = spreadCrystal4Data->Instantiate();
			Com_Transform* tr = spreadCrystal4Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(103.0f, 154.0f, 517.0f);
			tr->SetRelativeRotXYZ(0.0f, 3.1f, 3.1f);
			spreadCrystal4Obj->SetName("SpreadCrystal4");
			EventMgr::SpawnGameObject(spreadCrystal4Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> spreadCrystal3Data = ResMgr::Load<MeshData>("SpreadCrystal3(stm520_029_04).json");
			GameObject* spreadCrystal3Obj = spreadCrystal3Data->Instantiate();
			Com_Transform* tr = spreadCrystal3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-1447.0f, 870.0f, -989.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			spreadCrystal3Obj->SetName("SpreadCrystal3_2");
			EventMgr::SpawnGameObject(spreadCrystal3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> crystalGroundData = ResMgr::Load<MeshData>("CrystalGround(stm520_016_02).json");
			GameObject* crystalGroundObj = crystalGroundData->Instantiate();
			Com_Transform* tr = crystalGroundObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-1412.0f, -79.0f, 472.0f);
			tr->SetRelativeRotXYZ(0.0f, 3.1f, 3.1f);
			crystalGroundObj->SetName("CrystalGround");
			EventMgr::SpawnGameObject(crystalGroundObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> spreadCrystal3Data = ResMgr::Load<MeshData>("SpreadCrystal3(stm520_029_04).json");
			GameObject* spreadCrystal3Obj = spreadCrystal3Data->Instantiate();
			Com_Transform* tr = spreadCrystal3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-1915.0f, 1067.0f, 1560.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.1f, 3.3f);
			spreadCrystal3Obj->SetName("SpreadCrystal3");
			EventMgr::SpawnGameObject(spreadCrystal3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-173.0f, 57.0f, -803.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			stoneWallObj->SetName("StoneWall");
			EventMgr::SpawnGameObject(stoneWallObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj2 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj2->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(476.0f, 57.0f, 698.0f);
			tr->SetRelativeRotXYZ(0.0f, 3.6f, 3.1f);
			stoneWallObj2->SetName("StoneWall_2");
			EventMgr::SpawnGameObject(stoneWallObj2, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> brownPlatStoneData = ResMgr::Load<MeshData>("BrownPlatStone(stm520_020_00).json");
			GameObject* brownPlatStoneObj = brownPlatStoneData->Instantiate();
			Com_Transform* tr = brownPlatStoneObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(476.0f, 57.0f, 698.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			brownPlatStoneObj->SetName("BrownPlatStone");
			EventMgr::SpawnGameObject(brownPlatStoneObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> brownStone3Data = ResMgr::Load<MeshData>("BrownStone3(stm520_019_00).json");
			GameObject* brownStone3Obj = brownStone3Data->Instantiate();
			Com_Transform* tr = brownStone3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(447.0f, 56.0f, -404.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			brownStone3Obj->SetName("BrownStone3");
			EventMgr::SpawnGameObject(brownStone3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> brownStone3Data = ResMgr::Load<MeshData>("BrownStone3(stm520_019_00).json");
			GameObject* brownStone3Obj2 = brownStone3Data->Instantiate();
			Com_Transform* tr = brownStone3Obj2->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(699.0f, -4.0f, 121.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.0f, 3.1f);
			brownStone3Obj2->SetName("BrownStone3_2");
			EventMgr::SpawnGameObject(brownStone3Obj2, eLayerType::Stage);
		}


		{
			std::shared_ptr<MeshData> brownStone2Data = ResMgr::Load<MeshData>("BrownStone2(stm520_018_00).json");
			GameObject* brownStone2Obj = brownStone2Data->Instantiate();
			Com_Transform* tr = brownStone2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(526.0f, 20.0f, -118.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.0f, 3.1f);
			brownStone2Obj->SetName("BrownStone2");
			EventMgr::SpawnGameObject(brownStone2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> manyCrystalData = ResMgr::Load<MeshData>("ManyCrystal(evm057_00).json");
			GameObject* manyCrystalObj = manyCrystalData->Instantiate();
			Com_Transform* tr = manyCrystalObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-2199.0f, -21.0f, 1779.0f);
			tr->SetRelativeRotXYZ(0.0f, 4.7f, 3.1f);
			manyCrystalObj->SetName("ManyCrystal");
			EventMgr::SpawnGameObject(manyCrystalObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> manyCrystalData = ResMgr::Load<MeshData>("ManyCrystal(evm057_00).json");
			GameObject* manyCrystalObj2 = manyCrystalData->Instantiate();
			Com_Transform* tr = manyCrystalObj2->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-3326.0f, -44.0f, -653.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			manyCrystalObj2->SetName("ManyCrystal2");
			EventMgr::SpawnGameObject(manyCrystalObj2, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> groundData = ResMgr::Load<MeshData>("evm104_00.json");
			GameObject* groundObj = groundData->Instantiate();
			Com_Transform* tr = groundObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.1f));
			tr->SetRelativePos(-6782.0f, -1238.0f, -653.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			groundObj->SetName("Ground");
			EventMgr::SpawnGameObject(groundObj, eLayerType::Stage);
		}


		{
			std::shared_ptr<MeshData> groundData = ResMgr::Load<MeshData>("evm104_00.json");
			GameObject* groundObj2 = groundData->Instantiate();
			Com_Transform* tr = groundObj2->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.1f));
			tr->SetRelativePos(-6901.0f, -859.0f, 6123.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			groundObj2->SetName("Ground2");
			EventMgr::SpawnGameObject(groundObj2, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> groundData = ResMgr::Load<MeshData>("evm104_00.json");
			GameObject* groundObj3 = groundData->Instantiate();
			Com_Transform* tr = groundObj3->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.1f));
			tr->SetRelativePos(-7509.0f, -1842.0f, -7294.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			groundObj3->SetName("Ground3");
			EventMgr::SpawnGameObject(groundObj3, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> groundData = ResMgr::Load<MeshData>("evm104_00.json");
			GameObject* groundObj3 = groundData->Instantiate();
			Com_Transform* tr = groundObj3->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.1f));
			tr->SetRelativePos(-7509.0f, -1842.0f, -7294.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			groundObj3->SetName("Ground3");
			EventMgr::SpawnGameObject(groundObj3, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall6(stm520_008_00).json");
			GameObject* stoneWallObj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-1361.0f, -1185.0f, -4450.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			stoneWallObj->SetName("StoneWall6");
			EventMgr::SpawnGameObject(stoneWallObj, eLayerType::Stage);
		}


		{
			std::shared_ptr<MeshData> pointedCrystalData = ResMgr::Load<MeshData>("PointedCrystal(stm520_047_00).json");
			GameObject* pointedCrystalObj = pointedCrystalData->Instantiate();
			Com_Transform* tr = pointedCrystalObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-1155.0f, 812.0f, 1711.0f);
			tr->SetRelativeRotXYZ(-1.0f, 0.0f, 3.1f);
			pointedCrystalObj->SetName("PointedCrystal");
			EventMgr::SpawnGameObject(pointedCrystalObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWall2Data = ResMgr::Load<MeshData>("StoneWall2(stm520_004_00).json");
			GameObject* stoneWall2Obj = stoneWall2Data->Instantiate();
			Com_Transform* tr = stoneWall2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-1987.0f, -65.0f, -726.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			stoneWall2Obj->SetName("stoneWall2");
			EventMgr::SpawnGameObject(stoneWall2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWall2Data = ResMgr::Load<MeshData>("StoneWall2(stm520_004_00).json");
			GameObject* stoneWall2_2Obj = stoneWall2Data->Instantiate();
			Com_Transform* tr = stoneWall2_2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-1680.0f, 59.0f, 1399.0f);
			tr->SetRelativeRotXYZ(0.1f, 3.1f, 3.1f);
			stoneWall2_2Obj->SetName("stoneWall2_2");
			EventMgr::SpawnGameObject(stoneWall2_2Obj, eLayerType::Stage);
		}
		{
			std::shared_ptr<MeshData> stoneWall2Data = ResMgr::Load<MeshData>("StoneWall2(stm520_004_00).json");
			GameObject* stoneWall2_3Obj = stoneWall2Data->Instantiate();
			Com_Transform* tr = stoneWall2_3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.017f,0.01f,0.01f));
			tr->SetRelativePos(-2698.0f, 52.0f, 2484.0f);
			tr->SetRelativeRotXYZ(0.0f, 4.6f, 3.1f);
			stoneWall2_3Obj->SetName("stoneWall2_3");
			EventMgr::SpawnGameObject(stoneWall2_3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> pointedCrystalData = ResMgr::Load<MeshData>("PointedCrystal(stm520_047_00).json");
			GameObject* pointedCrystal_2Obj = pointedCrystalData->Instantiate();
			Com_Transform* tr = pointedCrystal_2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-2423.0f, -63.0f, 1418.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			pointedCrystal_2Obj->SetName("PointedCrystal2");
			EventMgr::SpawnGameObject(pointedCrystal_2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> bridgeData = ResMgr::Load<MeshData>("stm502_033_00.json");
			GameObject* bridge2Obj = bridgeData->Instantiate();
			Com_Transform* tr = bridge2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-2685.0f, -399.0f, -1807.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.5f, 3.1f);
			bridge2Obj->SetName("Bridge2");
			EventMgr::SpawnGameObject(bridge2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> bridgeData = ResMgr::Load<MeshData>("stm502_033_00.json");
			GameObject* bridge3Obj = bridgeData->Instantiate();
			Com_Transform* tr = bridge3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-2366.0f, 45.0f, -2434.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.5f, 3.1f);
			bridge3Obj->SetName("Bridge3");
			EventMgr::SpawnGameObject(bridge3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> pointedCrystalData = ResMgr::Load<MeshData>("PointedCrystal(stm520_047_00).json");
			GameObject* pointedCrystal_3Obj = pointedCrystalData->Instantiate();
			Com_Transform* tr = pointedCrystal_3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-3127.0f, -449.0f, -3301.0f);
			tr->SetRelativeRotXYZ(-0.3f, 0.0f, 3.1f);
			pointedCrystal_3Obj->SetName("PointedCrystal3");
			EventMgr::SpawnGameObject(pointedCrystal_3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> pointedCrystalData = ResMgr::Load<MeshData>("PointedCrystal(stm520_047_00).json");
			GameObject* pointedCrystal_4Obj = pointedCrystalData->Instantiate();
			Com_Transform* tr = pointedCrystal_4Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-2786.0f, -655.0f, -4131.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			pointedCrystal_4Obj->SetName("PointedCrystal4");
			EventMgr::SpawnGameObject(pointedCrystal_4Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> pointedCrystalData = ResMgr::Load<MeshData>("PointedCrystal(stm520_047_00).json");
			GameObject* pointedCrystal_5Obj = pointedCrystalData->Instantiate();
			Com_Transform* tr = pointedCrystal_5Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-2586.0f, -21.0f, -1160.0f);
			tr->SetRelativeRotXYZ(0.2f, 0.0f, 3.6f);
			pointedCrystal_5Obj->SetName("PointedCrystal5");
			EventMgr::SpawnGameObject(pointedCrystal_5Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> longPointedCrystalData = ResMgr::Load<MeshData>("LongPointedCrystal(stm520_028_00).json");
			GameObject* longPointedCrystalObj = longPointedCrystalData->Instantiate();
			Com_Transform* tr = longPointedCrystalObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-5402.0f, -183.0f, 1402.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.6f);
			longPointedCrystalObj->SetName("LongPointedCrystal");
			EventMgr::SpawnGameObject(longPointedCrystalObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> pointedCrystal4Data = ResMgr::Load<MeshData>("PointedCrystal4(stm550_003_00).json");
			GameObject* pointedCrystal4_1Obj = pointedCrystal4Data->Instantiate();
			Com_Transform* tr = pointedCrystal4_1Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-24388.0f, 215.0f, -1928.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			pointedCrystal4_1Obj->SetName("LongPointedCrystal4_1");
			EventMgr::SpawnGameObject(pointedCrystal4_1Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall6(stm520_008_00).json");
			GameObject* stoneWall6_2Obj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWall6_2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-14598.0f, -426.0f, 4263.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			stoneWall6_2Obj->SetName("StoneWall6_2");
			EventMgr::SpawnGameObject(stoneWall6_2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall6(stm520_008_00).json");
			GameObject* stoneWall6_3Obj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWall6_3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-12355.0f, 724.0f, 4299.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			stoneWall6_3Obj->SetName("StoneWall6_3");
			EventMgr::SpawnGameObject(stoneWall6_3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall6(stm520_008_00).json");
			GameObject* stoneWall6_4Obj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWall6_4Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-11631.0f, 58.0f, 3175.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			stoneWall6_4Obj->SetName("StoneWall6_4");
			EventMgr::SpawnGameObject(stoneWall6_4Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> blackStone2Data = ResMgr::Load<MeshData>("BlackStone2(stm520_001_00).json");
			GameObject* blackstone2_1Obj = blackStone2Data->Instantiate();
			Com_Transform* tr = blackstone2_1Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-11622.0f, -383.0f, 1805.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.6f, 3.1f);
			blackstone2_1Obj->SetName("BlackStone2_1");
			EventMgr::SpawnGameObject(blackstone2_1Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> blackStone6Data = ResMgr::Load<MeshData>("BlackStone6(stm520_017_00).json");
			GameObject* blackstone6_1Obj = blackStone6Data->Instantiate();
			Com_Transform* tr = blackstone6_1Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.019f, 0.019f, 0.014f));
			tr->SetRelativePos(-11989.0f, -736.0f, 841.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.4f, 3.1f);
			blackstone6_1Obj->SetName("BlackStone6_1");
			EventMgr::SpawnGameObject(blackstone6_1Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall6(stm520_008_00).json");
			GameObject* stoneWall6_5Obj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWall6_5Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-12765.0f, -614.0f, 5614.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			stoneWall6_5Obj->SetName("StoneWall6_5");
			EventMgr::SpawnGameObject(stoneWall6_5Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall6(stm520_008_00).json");
			GameObject* stoneWall6_6Obj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWall6_6Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-13843.0f, -727.0f, -7196.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			stoneWall6_6Obj->SetName("StoneWall6_6");
			EventMgr::SpawnGameObject(stoneWall6_6Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall6(stm520_008_00).json");
			GameObject* stoneWall6_7Obj = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWall6_7Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-13628.0f, -1433.0f, -8127.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			stoneWall6_7Obj->SetName("StoneWall6_7");
			EventMgr::SpawnGameObject(stoneWall6_7Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> blackStone2Data = ResMgr::Load<MeshData>("BlackStone2(stm520_001_00).json");
			GameObject* blackstone2_2Obj = blackStone2Data->Instantiate();
			Com_Transform* tr = blackstone2_2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.026f,0.022f,0.016f));
			tr->SetRelativePos(-12158.0f, -1120.0f, -4995.0f);
			tr->SetRelativeRotXYZ(0.0f, 5.2f, 3.1f);
			blackstone2_2Obj->SetName("BlackStone2_2");
			EventMgr::SpawnGameObject(blackstone2_2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> blackStone2Data = ResMgr::Load<MeshData>("BlackStone2(stm520_001_00).json");
			GameObject* blackstone2_3Obj = blackStone2Data->Instantiate();
			Com_Transform* tr = blackstone2_3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.018f,0.015f,0.012f));
			tr->SetRelativePos(-11622.0f, -766.0f, -2353.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.6f, 3.0f);
			blackstone2_3Obj->SetName("BlackStone2_3");
			EventMgr::SpawnGameObject(blackstone2_3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> blackStone2Data = ResMgr::Load<MeshData>("BlackStone2(stm520_001_00).json");
			GameObject* blackstone2_4Obj = blackStone2Data->Instantiate();
			Com_Transform* tr = blackstone2_4Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.012f, 0.013f, 0.012f));
			tr->SetRelativePos(-11875.0f, -446.0f, -453.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.9f, 3.0f);
			blackstone2_4Obj->SetName("BlackStone2_4");
			EventMgr::SpawnGameObject(blackstone2_4Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj3 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj3->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.028f,0.031f,0.016f));
			tr->SetRelativePos(-11701.0f, -1294.0f, -10110.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.9f, 3.1f);
			stoneWallObj3->SetName("StoneWall_3");
			EventMgr::SpawnGameObject(stoneWallObj3, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj4 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj4->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.028f,0.031f,0.02f));
			tr->SetRelativePos(-8544.0f, -1452.0f, -12160.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.3f, 3.1f);
			stoneWallObj4->SetName("StoneWall_4");
			EventMgr::SpawnGameObject(stoneWallObj4, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj5 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj5->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.021f,0.021f,0.012f));
			tr->SetRelativePos(-11485.0f, -60.0f, 7511.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.0f, 3.1f);
			stoneWallObj5->SetName("StoneWall_5");
			EventMgr::SpawnGameObject(stoneWallObj5, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj6 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj6->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.038f,0.026f,0.022f));
			tr->SetRelativePos(-4442.0f, -1147.0f, -11419.0f);
			tr->SetRelativeRotXYZ(0.0f, -0.6f, 3.14f);
			stoneWallObj6->SetName("StoneWall_6");
			EventMgr::SpawnGameObject(stoneWallObj6, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj7 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj7->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.039f,0.021f,0.025f));
			tr->SetRelativePos(-1810.0f, -863.0f, -7862.0f);
			tr->SetRelativeRotXYZ(0.0f, -1.4f, 3.1f);
			stoneWallObj7->SetName("StoneWall_7");
			EventMgr::SpawnGameObject(stoneWallObj7, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj8 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj8->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.032f,0.019f,0.01f));
			tr->SetRelativePos(-9269.0f, 381.0f, 9697.0f);
			tr->SetRelativeRotXYZ(0.0f, 2.6f, 3.1f);
			stoneWallObj8->SetName("StoneWall_8");
			EventMgr::SpawnGameObject(stoneWallObj8, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj9 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj9->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.025f,0.022f,0.01f));
			tr->SetRelativePos(-5442.0f, 317.0f, 9751.0f);
			tr->SetRelativeRotXYZ(0.0f, 3.7f, 3.1f);
			stoneWallObj9->SetName("StoneWall_9");
			EventMgr::SpawnGameObject(stoneWallObj9, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> stoneWallData = ResMgr::Load<MeshData>("StoneWall(stm520_003_00).json");
			GameObject* stoneWallObj10 = stoneWallData->Instantiate();
			Com_Transform* tr = stoneWallObj10->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.035f,0.024f,0.018f));
			tr->SetRelativePos(-3322.0f, 241.0f, 6208.0f);
			tr->SetRelativeRotXYZ(0.0f, -1.7f, 3.1f);
			stoneWallObj10->SetName("StoneWall_10");
			EventMgr::SpawnGameObject(stoneWallObj10, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> spikyStoneData = ResMgr::Load<MeshData>("SpikyStone(evm090_00).json");
			GameObject* spikyStoneObj = spikyStoneData->Instantiate();
			Com_Transform* tr = spikyStoneObj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-7458.0f, -167.0f, -509.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.1f);
			spikyStoneObj->SetName("SpikyStone");
			EventMgr::SpawnGameObject(spikyStoneObj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> longPointedCrystalData = ResMgr::Load<MeshData>("LongPointedCrystal(stm520_028_00).json");
			GameObject* longPointedCrystal_2Obj = longPointedCrystalData->Instantiate();
			Com_Transform* tr = longPointedCrystal_2Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-4678.0f, -767.0f, -5797.0f);
			tr->SetRelativeRotXYZ(0.5f, 0.7f, 3.6f);
			longPointedCrystal_2Obj->SetName("LongPointedCrystal_2");
			EventMgr::SpawnGameObject(longPointedCrystal_2Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> longPointedCrystalData = ResMgr::Load<MeshData>("LongPointedCrystal(stm520_028_00).json");
			GameObject* longPointedCrystal_3Obj = longPointedCrystalData->Instantiate();
			Com_Transform* tr = longPointedCrystal_3Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-11268.0f, -917.0f, -4675.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.6f);
			longPointedCrystal_3Obj->SetName("LongPointedCrystal_3");
			EventMgr::SpawnGameObject(longPointedCrystal_3Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> longPointedCrystalData = ResMgr::Load<MeshData>("LongPointedCrystal(stm520_028_00).json");
			GameObject* longPointedCrystal_4Obj = longPointedCrystalData->Instantiate();
			Com_Transform* tr = longPointedCrystal_4Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-11534.0f, -985.0f, -5045.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.0f);
			longPointedCrystal_4Obj->SetName("LongPointedCrystal_4");
			EventMgr::SpawnGameObject(longPointedCrystal_4Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> longPointedCrystalData = ResMgr::Load<MeshData>("LongPointedCrystal(stm520_028_00).json");
			GameObject* longPointedCrystal_5Obj = longPointedCrystalData->Instantiate();
			Com_Transform* tr = longPointedCrystal_5Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-8809.0f, -376.0f, 3993.0f);
			tr->SetRelativeRotXYZ(0.0f, 1.0f, 3.2f);
			longPointedCrystal_5Obj->SetName("LongPointedCrystal_5");
			EventMgr::SpawnGameObject(longPointedCrystal_5Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> longPointedCrystalData = ResMgr::Load<MeshData>("LongPointedCrystal(stm520_028_00).json");
			GameObject* longPointedCrystal_6Obj = longPointedCrystalData->Instantiate();
			Com_Transform* tr = longPointedCrystal_6Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-5402.0f, -94.0f, 7586.0f);
			tr->SetRelativeRotXYZ(0.0f, 0.0f, 3.6f);
			longPointedCrystal_6Obj->SetName("LongPointedCrystal_6");
			EventMgr::SpawnGameObject(longPointedCrystal_6Obj, eLayerType::Stage);
		}

		{
			std::shared_ptr<MeshData> longPointedCrystalData = ResMgr::Load<MeshData>("LongPointedCrystal(stm520_028_00).json");
			GameObject* longPointedCrystal_7Obj = longPointedCrystalData->Instantiate();
			Com_Transform* tr = longPointedCrystal_7Obj->GetComponent<Com_Transform>();
			tr->SetRelativeScale(float3(0.01f));
			tr->SetRelativePos(-5402.0f, -313.0f, 4456.0f);
			tr->SetRelativeRotXYZ(0.4f, 0.1f, 3.0f);
			longPointedCrystal_7Obj->SetName("LongPointedCrystal_7");
			EventMgr::SpawnGameObject(longPointedCrystal_7Obj, eLayerType::Stage);
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
			
			//GameObject* directionalLight = object::Instantiate(eLayerType::Player, new GameObject);
			GameObject* directionalLight = EventMgr::SpawnGameObject(new GameObject, eLayerType::Player);
			directionalLight->SetName("directionalLight");

			Com_Transform* tr = directionalLight->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(0.0f, 100.0f, 0.0f));
			tr->SetRelativeRotXYZ(float3(45.0f, 0.0f, 0.0f));

			Com_Light3D* lightComp = directionalLight->AddComponent<Com_Light3D>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(float4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(float4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(float4(0.4f, 0.4f, 0.4f, 1.0f));
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
