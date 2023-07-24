#pragma once
#include "guiWidget.h"
#include "GameObject.h"


namespace gui
{
	class IComponent : public Widget
	{
	public:
		IComponent(mh::enums::eComponentType _type);
		virtual ~IComponent();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetTarget(mh::GameObject* _target) { mTarget = _target; }
		mh::GameObject* GetTarget() { return mTarget; }

	private:
		mh::enums::eComponentType mType;
		mh::GameObject* mTarget;
	};
}
