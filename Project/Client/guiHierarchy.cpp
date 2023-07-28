#include "ClientPCH.h"
#include "guiHierarchy.h"

#include <Engine/Application.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/SceneManager.h>
#include <Engine/RenderMgr.h>

#include "guiInspector.h"
#include "guiEditor.h"
#include "guiInspector.h"

extern mh::Application application;
extern gui::Editor editor;

namespace gui
{
	Hierarchy::Hierarchy()
		: mTreeWidget(nullptr)
	{
		SetKey("Hierarchy");
		SetSize(ImVec2(1600 / 2, 900 / 2));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetKey("Scenes");
		AddWidget(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&Hierarchy::InitializeInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeScene();
	}

	Hierarchy::~Hierarchy()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}

	void Hierarchy::FixedUpdate()
	{
	}

	void Hierarchy::Update()
	{
	}

	void Hierarchy::LateUpdate()
	{
	}

	void Hierarchy::InitializeInspector(void* data)
	{
		mh::RenderMgr::GetInst()->SetInspectorGameObject(static_cast<mh::GameObject*>(data));

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(mh::RenderMgr::GetInst()->GetInspectorGameObject());
		inspector->InitializeTargetGameObject();


	}

	void Hierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		mh::Scene* scene = mh::SceneManager::GetActiveScene();
		std::string sceneName = scene->GetKey();

		TreeWidget::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);

		for (size_t i = 0; i < (UINT)mh::define::eLayerType::End; i++)
		{
			mh::Layer& layer = scene->GetLayer((mh::define::eLayerType)i);
			const std::vector<mh::GameObject*>& gameObjs
				= layer.GetGameObjects();

			for (mh::GameObject* obj : gameObjs)
			{
				AddGameObject(root, obj);
			}
		}

	}

	void Hierarchy::AddGameObject(TreeWidget::tNode* parent, mh::GameObject* gameObject)
	{
		std::string name(gameObject->GetName().begin(), gameObject->GetName().end());

		TreeWidget::tNode* node = mTreeWidget->AddNode(parent, name, gameObject);
	}

}
