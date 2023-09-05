#pragma once
#include "IScript.h"


namespace mh
{
	class Script_Player : public IScript
	{
	public:
		Script_Player();
		Script_Player(const Script_Player& _ohter) = default;
		CLONE(Script_Player);

		virtual ~Script_Player();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

		virtual void OnCollisionEnter(ICollider2D* _collider) override;
		virtual void OnCollisionStay(ICollider2D* _collider) override;
		virtual void OnCollisionExit(ICollider2D* _collider) override;

		void Start();
		void Action();
		void End();

	private:
		float mMoveSpeed;
	};
}
