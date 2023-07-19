#pragma once
#include "guiWidget.h"
#include "GameObject.h"
#include "guiComponent.h"
#include "guiResource.h"

namespace gui
{
	class Inspector : public Widget
	{
	public:
		Inspector();
		~Inspector();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		mh::GameObject* GetTargetGameObject() const { return mTargetGameObject; }
		void SetTargetGameObject(mh::GameObject* target) { mTargetGameObject = target; }
		ya::Resource* GetTargetResource() const { return mTargetResource; }
		void SetTargetResource(ya::Resource* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

	private:
		mh::GameObject* mTargetGameObject;
		ya::Resource* mTargetResource;
		std::vector<gui::IComponent*> mComponents;
		std::vector<gui::Resource*> mResources;
	};
}