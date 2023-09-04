#pragma once
#include <Engine\IScene.h>

namespace mh
{
	class Scene_TestSkyBox : public IScene
	{
	public:
		// construtor destructor
		Scene_TestSkyBox();
		~Scene_TestSkyBox();

		// delete Function
		Scene_TestSkyBox(const Scene_TestSkyBox& _Other) = delete;
		Scene_TestSkyBox(Scene_TestSkyBox&& _Other) noexcept = delete;
		Scene_TestSkyBox& operator=(const Scene_TestSkyBox& _Other) = delete;
		Scene_TestSkyBox& operator=(Scene_TestSkyBox&& _Other) noexcept = delete;

	public:
		void OnEnter() override;
		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;
		void Destroy() override;
		void OnExit() override;

	protected:


	private:

	};
}
