#include "PCH_Engine.h"

#include "IScene.h"


namespace mh
{
	IScene::IScene()
		: mbInitialized()
	{
	}

	IScene::~IScene()
	{
	}

	void IScene::SceneInit()
	{
		Init();
		for (Layer& layer : mLayers)
		{
			layer.Init();
		}

		mbInitialized = true;
	}
	void IScene::SceneUpdate()
	{
		Update();

		for (Layer& layer : mLayers)
		{
			layer.Update();
		}
	}
	void IScene::SceneFixedUpdate()
	{
		FixedUpdate();
		for (Layer& layer : mLayers)
		{
			layer.FixedUpdate();
		}
	}
	void IScene::SceneRender()
	{
		Render();
		for (Layer& layer : mLayers)
		{
			layer.Render();
		}
	}
	void IScene::SceneDestroy()
	{
		Destroy();
		for (Layer& layer : mLayers)
		{
			layer.Destroy();
		}
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
