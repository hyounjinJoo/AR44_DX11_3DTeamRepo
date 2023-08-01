#pragma once
#include "IScript.h"


namespace mh
{
	class Script_Player : public IScript
	{
	public:
		Script_Player();
		virtual ~Script_Player();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(ICollider2D* _collider) override;
		virtual void OnCollisionStay(ICollider2D* _collider) override;
		virtual void OnCollisionExit(ICollider2D* _collider) override;

		void Start();
		void Action();
		void End();

	private:
	};
}
