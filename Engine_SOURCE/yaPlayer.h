#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Player : public GameObject
	{
	public:
		Player();
		virtual ~Player();

		virtual void Initalize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		//GameObject* head;
		//GameObject* body;
	};
}
