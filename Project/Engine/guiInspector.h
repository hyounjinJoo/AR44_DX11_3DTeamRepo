#pragma once
#include "guiChild.h"
#include "GameObject.h"
#include "guiComponent.h"
#include "guiResource.h"

namespace gui
{
	class guiInspector : public guiWindow
	{
	public:
		guiInspector();
		virtual ~guiInspector();

		virtual void Init() override;
		virtual void Update() override;
		virtual void UpdateUI() override;

		void SetTargetGameObject(mh::GameObject* _targetObj) { mTargetGameObject = _targetObj; }
		mh::GameObject* GetTargetGameObject() const { return mTargetGameObject; }

		void SetTargetResource(mh::IRes* _targetRes) { mTargetResource = _targetRes; }
		mh::IRes* GetTargetResource() const { return mTargetResource; }


	private:
		void IndicatorButton(const char* _strButtonName);

	private:
		mh::GameObject* mTargetGameObject;
		mh::IRes* mTargetResource;

		std::vector<gui::guiComponent*> mGuiComponents;
		std::vector<gui::guiResource*> mGuiResources;
	};
}
