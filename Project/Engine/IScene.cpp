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

	GameObject* IScene::AddGameObject(GameObject* _gameObj, const define::eLayerType _type)
	{
		if (_gameObj)
		{
			if (define::eLayerType::None == _type)
			{
				ERROR_MESSAGE_W(L"레이어를 설정하지 않았습니다.");
				SAFE_DELETE(_gameObj);
				MH_ASSERT(_gameObj);
			}
			else
			{
				std::vector<GameObject*> gameObjs{};
				_gameObj->GetGameObjectHierarchy(gameObjs);

				for (size_t i = 0; i < gameObjs.size(); ++i)
				{
					eLayerType type = gameObjs[i]->GetLayerType();
					if (eLayerType::None != type)
					{
						mLayers[(int)type].RemoveGameObject(gameObjs[i]);
					}

					gameObjs[i]->SetLayerType(_type);
					mLayers[(int)_type].AddGameObject(gameObjs[i], mbInitialized);
				}
			}
		}
		return _gameObj;
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
