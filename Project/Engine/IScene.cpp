#include "PCH_Engine.h"

#include "IScene.h"


namespace mh
{
	IScene::IScene()
		: mbInitialized()
	{
		mLayers.resize((uint)define::eLayerType::END);
	}

	IScene::~IScene()
	{

	}

	void IScene::Init()
	{
		mbInitialized = true;
		for (Layer& layer : mLayers)
		{
			layer.Init();
		}
	}
	void IScene::Update()
	{
		for (Layer& layer : mLayers)
		{
			layer.Update();
		}
	}
	void IScene::FixedUpdate()
	{
		for (Layer& layer : mLayers)
		{
			layer.FixedUpdate();
		}
	}
	void IScene::Render()
	{
		for (Layer& layer : mLayers)
		{
			layer.Render();
		}
	}
	void IScene::Destroy()
	{
		for (Layer& layer : mLayers)
		{
			layer.Destroy();
		}
	}

	void IScene::OnExit()
	{
	}

	void IScene::AddGameObject(GameObject* _gameObj, const define::eLayerType _type)
	{
		mLayers[(uint)_type].AddGameObject(_gameObj, mbInitialized);
		_gameObj->SetLayerType(_type);
	}
	
	std::vector<GameObject*> IScene::GetDontDestroyGameObjects()
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
	const std::vector<GameObject*>& IScene::GetGameObjects(const define::eLayerType _type)
	{
		return mLayers[(uint)_type].GetGameObjects();
	}
}
