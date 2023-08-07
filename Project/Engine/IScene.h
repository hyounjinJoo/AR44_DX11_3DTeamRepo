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

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		//리소스 로드
		virtual void OnEnter() = 0;
		virtual void OnExit();

		void							AddGameObject(GameObject* _gameObj, const define::eLayerType _type);
		Layer&							GetLayer(define::eLayerType _type) { return mLayers[(uint)_type]; }
		std::vector<GameObject*>		GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const define::eLayerType _type);


	private:
		std::vector<Layer> mLayers;
	};
}
