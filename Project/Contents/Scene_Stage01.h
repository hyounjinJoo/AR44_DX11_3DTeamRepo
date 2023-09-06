#pragma once
#include <Engine/IScene.h>

namespace mh
{
	class Scene_Stage01
		: public IScene
	{
	public:
		Scene_Stage01();
		virtual ~Scene_Stage01();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
	};
}
