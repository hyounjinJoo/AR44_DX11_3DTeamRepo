#pragma once
#include "Entity.h"
#include "Layer.h"

namespace mh
{
	class GameObject;
	class IScene 
		: public Entity
	{
	public:
		IScene();
		virtual ~IScene();

		//실제 로직은 여기서
		void SceneInit();
		void SceneUpdate();
		void SceneFixedUpdate();
		void SceneRender();
		void SceneDestroy();


		//아래 함수를 재정의해서 씬을 커스터마이즈 하면 됨
		virtual void OnEnter() = 0;//리소스 로드
		virtual void Init() {};//몬스터 배치
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void Render() {};
		virtual void Destroy() {};
		virtual void OnExit() {};
		
		//Add 'New' Game Object -> 이미 레이어에 들어갔었던 게임오브젝트는 이 함수를 사용하면 안됨
		inline void	AddGameObject(const define::eLayerType _type, GameObject* _gameObj);
		inline void	AddGameObjectHierarchy(const define::eLayerType _type, GameObject* _gameObj);

		inline void	MoveGameObjectLayer(const define::eLayerType _targetLayer, GameObject* _gameObj);
		inline void	MoveGameObjectLayerHierarchy(const define::eLayerType _targetLayer, GameObject* _gameObj);

		Layer&							GetLayer(define::eLayerType _type) { return mLayers[(uint)_type]; }
		std::vector<GameObject*>		GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const define::eLayerType _type);


		bool	IsInitialized() const { return mbInitialized; }



	private:
		std::array<Layer, (int)define::eLayerType::END> mLayers;
		bool mbInitialized;
	};

	inline void IScene::AddGameObject(const define::eLayerType _type, GameObject* _gameObj)
	{
		MH_ASSERT(eLayerType::None != _type && _gameObj);
		GetLayer(_type).AddGameObject(_gameObj, mbInitialized);
	}

	inline void IScene::AddGameObjectHierarchy(const define::eLayerType _type, GameObject* _gameObj)
	{
		MH_ASSERT(eLayerType::None != _type && _gameObj);

		std::vector<GameObject*> gameObjs{};
		_gameObj->GetGameObjectHierarchy(gameObjs);
		for (size_t i = 0; i < gameObjs.size(); ++i)
		{
			AddGameObject(_type, gameObjs[i]);
		}
	}

	inline void IScene::MoveGameObjectLayer(const define::eLayerType _targetLayer, GameObject* _gameObj)
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

	inline void IScene::MoveGameObjectLayerHierarchy(const define::eLayerType _targetLayer, GameObject* _gameObj)
	{
		MH_ASSERT(eLayerType::None != _targetLayer && _gameObj);

		std::vector<GameObject*> gameObjs{};
		_gameObj->GetGameObjectHierarchy(gameObjs);
		for (size_t i = 0; i < gameObjs.size(); ++i)
		{
			MoveGameObjectLayer(_targetLayer, gameObjs[i]);
		}
	}

}
