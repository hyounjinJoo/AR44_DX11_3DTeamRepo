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


	void IScene::MoveGameObjectLayer(const define::eLayerType _targetLayer, GameObject* _gameObj)
	{
		MH_ASSERT(eLayerType::None != _targetLayer && _gameObj);

		eLayerType prevLayer = _gameObj->GetLayerType();
		if (define::eLayerType::None != prevLayer)
		{
			GetLayer(prevLayer).RemoveGameObject(_gameObj);
		}

		//새 레이어에 넣어준다.
		//Move이므로 초기화 함수는 호출하지 않음
		GetLayer(_targetLayer).AddGameObject(_gameObj, false);
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
