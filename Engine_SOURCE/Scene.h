#pragma once
#include "Entity.h"
#include "Layer.h"
#include "GameObject.h"

namespace mh
{
	class Scene : public Entity
	{
	public:
		Scene(enums::eSceneType type);
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		enums::eSceneType				GetSceneType() { return mType; }
		void							AddGameObject(GameObject* _gameObj, const enums::eLayerType _type);
		Layer&							GetLayer(enums::eLayerType _type) { return mLayers[(UINT)_type]; }
		std::vector<GameObject*>		GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const enums::eLayerType _type);


	private:
		std::vector<Layer> mLayers;
		enums::eSceneType mType;
	};
}
