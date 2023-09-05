#pragma once
#include "GameObject.h"

namespace mh
{
	class Layer : public Entity
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		void AddGameObject(GameObject* gameObject);
		void RemoveGameObject(const GameObject* gameObject);
		const std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }
		std::vector<GameObject*> GetDontDestroyGameObjects();

		void SetLayerType(define::eLayerType _type) { mLayerType = _type; }

	private:
		define::eLayerType mLayerType;
		std::vector<GameObject*> mGameObjects;
	};

	typedef const std::vector<GameObject*>& GameObjects;
	typedef std::vector<GameObject*>::iterator GameObjectIter;
}
