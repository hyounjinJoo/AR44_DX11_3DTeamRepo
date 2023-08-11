#pragma once
#include "guiChild.h"

#include "guiTreeWidget.h"

namespace mh
{
	class GameObject;
}

namespace gui
{
	class guiTree_GameObject : public guiWindow
	{
	public:
		guiTree_GameObject();
		virtual ~guiTree_GameObject();

		virtual void Update() override;

		void GameobjectSelectCallback(mh::define::tDataPtr _data);
		void InitializeScene();
		void AddGameObject(TreeWidget::tNode* parent, mh::GameObject* gameObject);

	private:
		TreeWidget* mTreeWidget;
	};
}
