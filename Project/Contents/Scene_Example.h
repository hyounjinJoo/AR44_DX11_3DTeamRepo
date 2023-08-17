#pragma once

#include <Engine/IScene.h>

namespace mh
{
	class Scene_Example :
		public IScene
	{
	public:
		Scene_Example();
		virtual ~Scene_Example();

		virtual void OnEnter() override;
		virtual void Init() override;

		

	private:

	};
}


