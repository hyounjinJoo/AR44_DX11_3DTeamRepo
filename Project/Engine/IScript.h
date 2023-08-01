#pragma once
#include "IComponent.h"
#include "ICollider2D.h"

namespace mh
{
	//TODO: 순수가상함수 추가해서 인터페이스로 바꾸기
	class IScript : public IComponent
	{
	public:
		IScript();
		virtual ~IScript();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(ICollider2D* _collider) {};
		virtual void OnCollisionStay(ICollider2D* _collider) {};
		virtual void OnCollisionExit(ICollider2D* _collider) {};

		virtual void OnTriggerEnter(ICollider2D* _collider) {};
		virtual void OnTriggerStay(ICollider2D* _collider) {};
		virtual void OnTriggerExit(ICollider2D* _collider) {};

	private:
	};
}
