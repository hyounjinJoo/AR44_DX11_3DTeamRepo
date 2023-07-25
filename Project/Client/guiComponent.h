#pragma once
#include "guiWidget.h"
#include <Engine/GameObject.h>


namespace gui
{
	class IComponent : public Widget
	{
	public:
		IComponent(mh::define::eComponentType _type);
		virtual ~IComponent();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetTarget(mh::GameObject* _target) { mTarget = _target; }
		mh::GameObject* GetTarget() { return mTarget; }

	private:
		mh::define::eComponentType mType;
		mh::GameObject* mTarget;
	};
}
