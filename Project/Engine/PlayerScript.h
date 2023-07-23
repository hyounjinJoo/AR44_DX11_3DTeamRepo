#pragma once
#include "Script.h"


namespace mh
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		~PlayerScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* _collider) override;
		virtual void OnCollisionStay(Collider2D* _collider) override;
		virtual void OnCollisionExit(Collider2D* _collider) override;

		void Start();
		void Action();
		void End();

	private:
	};
}
