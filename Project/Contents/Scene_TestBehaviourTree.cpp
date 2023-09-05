#include "PCH_Contents.h"

#include "Scene_TestBehaviourTree.h"
#include <Engine\EventMgr.h>
#include <Engine\Com_BehaviorTree.h>
#include <Engine\BrainTree\example\Example01_Simple.cpp>

namespace mh
{
	Scene_TestBehaviourTree::Scene_TestBehaviourTree()
		: IScene()
	{
	}

	Scene_TestBehaviourTree::~Scene_TestBehaviourTree()
	{
	}

	void Scene_TestBehaviourTree::OnEnter()
	{
	}

	void Scene_TestBehaviourTree::Init()
	{
		GameObject* testObj = EventMgr::SpawnGameObject(eLayerType::Player);
		Com_BehaviorTree* testBT = testObj->AddComponent<Com_BehaviorTree>();

		auto repeater = std::make_shared<BrainTree::Repeater>(5);
		repeater->setChild(std::make_shared<BrainTree::SayHello>());
		testBT->GetBT()->setRoot(repeater);

		IScene::Init();
	}

	void Scene_TestBehaviourTree::Update()
	{
		IScene::Update();
	}

	void Scene_TestBehaviourTree::FixedUpdate()
	{
		IScene::FixedUpdate();
	}

	void Scene_TestBehaviourTree::Render()
	{
		IScene::Render();
	}

	void Scene_TestBehaviourTree::Destroy()
	{
		IScene::Destroy();
	}

	void Scene_TestBehaviourTree::OnExit()
	{
		IScene::OnExit();
	}
}
