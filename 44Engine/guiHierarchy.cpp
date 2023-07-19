#include "guiHierarchy.h"
#include "Application.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneManager.h"
#include "guiInspector.h"
#include "guiEditor.h"
#include "Renderer.h"
#include "guiInspector.h"

extern mh::Application application;
extern gui::Editor editor;

namespace gui
{
	Hierarchy::Hierarchy()
		: mTreeWidget(nullptr)
	{
		SetName("Hierarchy");
		SetSize(ImVec2(1600 / 2, 900 / 2));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Scenes");
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
		mh::renderer::gInspectorGameObject 
			= static_cast<mh::GameObject*>(data);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(mh::renderer::gInspectorGameObject);
		inspector->InitializeTargetGameObject();


	}

	void Hierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		ya::Scene* scene = ya::SceneManager::GetActiveScene();
		std::string sceneName(scene->GetName().begin(), scene->GetName().end());

		TreeWidget::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, 0, true);

		for (size_t i = 0; i < (UINT)mh::enums::eLayerType::End; i++)
		{
			ya::Layer& layer = scene->GetLayer((mh::enums::eLayerType)i);
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
