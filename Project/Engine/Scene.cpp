#include "EnginePCH.h"

#include "Scene.h"


namespace mh
{
	Scene::Scene(enums::eSceneType _type)
		: mType(_type)
	{
		mLayers.resize((UINT)enums::eLayerType::End);
	}

	Scene::~Scene()
	{

	}

	void Scene::Initialize()
	{
		for (Layer& layer : mLayers)
		{
			layer.Initialize();
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
	void Scene::AddGameObject(GameObject* _gameObj, const enums::eLayerType _type)
	{
		mLayers[(UINT)_type].AddGameObject(_gameObj);
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
	const std::vector<GameObject*>& Scene::GetGameObjects(const enums::eLayerType _type)
	{
		return mLayers[(UINT)_type].GetGameObjects();
	}
}
