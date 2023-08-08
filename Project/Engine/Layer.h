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

		void AddGameObject(GameObject* gameObject, bool _bNeedInit = false);
		const std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }
		std::vector<GameObject*> GetDontDestroyGameObjects();


	private:
		std::vector<GameObject*> mGameObjects;
	};

	typedef const std::vector<GameObject*>& GameObjects;
	typedef std::vector<GameObject*>::iterator GameObjectIter;
}
