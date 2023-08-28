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
		
		GameObject*	AddGameObject(GameObject* _gameObj, const define::eLayerType _type);

		Layer&							GetLayer(define::eLayerType _type) { return mLayers[(uint)_type]; }
		std::vector<GameObject*>		GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const define::eLayerType _type);

		bool	IsInitialized() const { return mbInitialized; }



	private:
		std::array<Layer, (int)define::eLayerType::END> mLayers;
		bool mbInitialized;
	};
}
