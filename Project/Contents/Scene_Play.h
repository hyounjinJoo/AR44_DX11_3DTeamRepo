#pragma once
#include <Engine/IScene.h>

namespace mh
{
	class Scene_Play 
		: public IScene
	{
	public:
		Scene_Play();
		virtual ~Scene_Play();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
	};
}
