#include "EnginePCH.h"

#include "Scene.h"


namespace mh
{
	Scene::Scene(define::eSceneType _type)
		: mType(_type)
	{
		mLayers.resize((uint)define::eLayerType::End);
	}

	Scene::~Scene()
	{

	}

	void Scene::Init()
	{
		for (Layer& layer : mLayers)
		{
			layer.Init();
		}
	}
	void Scene::Update()
	{
		for (Layer& layer : mLayers)
		{
			layer.Update();
		}
	}
	void Scene::FixedUpdate()
	{
		for (Layer& layer : mLayers)
		{
			layer.FixedUpdate();
		}
	}
	void Scene::Render()
	{
		for (Layer& layer : mLayers)
		{
			layer.Render();
		}
	}
	void Scene::Destroy()
	{
		for (Layer& layer : mLayers)
		{
			layer.Destroy();
		}
	}
	void Scene::OnEnter()
	{
	}
	void Scene::OnExit()
	{
	}
	void Scene::AddGameObject(GameObject* _gameObj, const define::eLayerType _type)
	{
		mLayers[(uint)_type].AddGameObject(_gameObj);
		_gameObj->SetLayerType(_type);
	}
	
	std::vector<GameObject*> Scene::GetDontDestroyGameObjects()
	{
		std::vector<GameObject*> gameObjects;
		for (Layer& layer : mLayers)
		{
			std::vector<GameObject*> dontGameObjs
				= layer.GetDontDestroyGameObjects();

			gameObjects.insert(gameObjects.end()
			, dontGameObjs.begin()
			, dontGameObjs.end());
		}

		return gameObjects;
	}
	const std::vector<GameObject*>& Scene::GetGameObjects(const define::eLayerType _type)
	{
		return mLayers[(uint)_type].GetGameObjects();
	}
}
