#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Monster : public GameObject
	{
	public:
		Monster();
		virtual ~Monster();

		virtual void Initalize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
	};
}