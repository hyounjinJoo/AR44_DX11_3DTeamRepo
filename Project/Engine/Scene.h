#pragma once
#include "Entity.h"
#include "Layer.h"
#include "GameObject.h"

namespace mh
{
	class Scene : public Entity
	{
	public:
		Scene(define::eSceneType type);
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		define::eSceneType				GetSceneType() { return mType; }
		void							AddGameObject(GameObject* _gameObj, const define::eLayerType _type);
		Layer&							GetLayer(define::eLayerType _type) { return mLayers[(UINT)_type]; }
		std::vector<GameObject*>		GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const define::eLayerType _type);


	private:
		std::vector<Layer> mLayers;
		define::eSceneType mType;
	};
}
