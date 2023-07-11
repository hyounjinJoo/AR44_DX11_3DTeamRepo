#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"



namespace gui
{
	class Component : public Widget
	{
	public:
		Component(ya::enums::eComponentType type);
		~Component();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetTarget(ya::GameObject* target) { mTarget = target; }
		ya::GameObject* GetTarget() { return mTarget; }

	private:
		ya::enums::eComponentType mType;
		ya::GameObject* mTarget;
	};
}
