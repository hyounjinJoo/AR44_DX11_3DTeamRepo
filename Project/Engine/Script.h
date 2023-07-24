#pragma once
#include "IComponent.h"
#include "Collider2D.h"

namespace mh
{
	class Script : public IComponent
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(Collider2D* _collider) {};
		virtual void OnCollisionStay(Collider2D* _collider) {};
		virtual void OnCollisionExit(Collider2D* _collider) {};

		virtual void OnTriggerEnter(Collider2D* _collider) {};
		virtual void OnTriggerStay(Collider2D* _collider) {};
		virtual void OnTriggerExit(Collider2D* _collider) {};

	private:
	};
}
