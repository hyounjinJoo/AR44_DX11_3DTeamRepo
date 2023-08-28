#include "PCH_Engine.h"

#include "IScene.h"


namespace mh
{
	IScene::IScene()
		: mbInitialized()
	{
		for (size_t i = 0; i < mLayers.size(); ++i)
		{
			mLayers[i].SetLayerType((eLayerType)i);
		}
	}

	IScene::~IScene()
	{
	}

	void IScene::SceneInit()
	{
		if (mbInitialized)
			return;
		mbInitialized = true;
		Init();
		for (Layer& layer : mLayers)
		{
			layer.Init();
		}
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
