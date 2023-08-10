#pragma once
#include "guiChild.h"

#include "guiTreeWidget.h"

namespace mh
{
	class GameObject;
}

namespace gui
{
	class guiTree_GameObject : public guiChild
	{
	public:
		guiTree_GameObject();
		virtual ~guiTree_GameObject();

		virtual void Update() override;

		void InitializeInspector(tData _data);
		void InitializeScene();
		void AddGameObject(TreeWidget::tNode* parent, mh::GameObject* gameObject);

	private:
		TreeWidget* mTreeWidget;
	};
}
