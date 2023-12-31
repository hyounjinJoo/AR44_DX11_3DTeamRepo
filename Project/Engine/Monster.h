#pragma once
#include "GameObject.h"

namespace mh
{
	class Monster : public GameObject
	{
	public:
		Monster();
		virtual ~Monster();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
	};
}