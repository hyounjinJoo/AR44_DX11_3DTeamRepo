#pragma once
#include <Engine\IScene.h>

namespace mh
{
	class Scene_TestBehaviourTree
		: public IScene
	{
	public:
		// construtor destructor
		Scene_TestBehaviourTree();
		virtual ~Scene_TestBehaviourTree();

		// delete Function
		Scene_TestBehaviourTree(const Scene_TestBehaviourTree& _Other) = delete;
		Scene_TestBehaviourTree(Scene_TestBehaviourTree&& _Other) noexcept = delete;
		Scene_TestBehaviourTree& operator=(const Scene_TestBehaviourTree& _Other) = delete;
		Scene_TestBehaviourTree& operator=(Scene_TestBehaviourTree&& _Other) noexcept = delete;


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
