#pragma once
#include "guiChild.h"
#include <Engine/GameObject.h>
#include "guiComponent.h"
#include "guiResource.h"

namespace gui
{
	class guiInspector : public guiChild
	{
	public:
		guiInspector();
		virtual ~guiInspector();

		virtual void Update() override;

		mh::GameObject* GetTargetGameObject() const { return mTargetGameObject; }
		void SetTargetGameObject(mh::GameObject* target) { mTargetGameObject = target; }
		mh::IRes* GetTargetResource() const { return mTargetResource; }
		void SetTargetResource(mh::IRes* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

	private:
		mh::GameObject* mTargetGameObject;
		mh::IRes* mTargetResource;

		guiComponent* mTransform;
		std::vector<gui::guiComponent*> mComponents;
		std::vector<gui::IRes*> mResources;
	};
}
