#include "PCH_Contents.h"

#include "Scene_TestBehaviourTree.h"
#include <Engine\EventMgr.h>

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
		GameObject* testObj = EventMgr::SpawnGameObject(new GameObject, eLayerType::Player);


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
