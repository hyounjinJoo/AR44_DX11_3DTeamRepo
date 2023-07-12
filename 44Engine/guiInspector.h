#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
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

		ya::GameObject* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(ya::GameObject* target) { mTargetGameObject = target; }
		ya::Resource* GetTargetResource() { return mTargetResource; }
		void SetTargetResource(ya::Resource* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

	private:
		ya::GameObject* mTargetGameObject;
		ya::Resource* mTargetResource;
		std::vector<gui::Component*> mComponents;
		std::vector<gui::Resource*> mResources;
	};
}