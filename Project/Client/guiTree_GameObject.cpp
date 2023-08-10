#include "PCH_Client.h"
#include "guiTree_GameObject.h"

#include <Engine/Application.h>
#include <Engine/IScene.h>
#include <Engine/Layer.h>
#include <Engine/SceneMgr.h>
#include <Engine/RenderMgr.h>

#include "guiInspector.h"
#include "ImGuiMgr.h"
#include "guiInspector.h"

#include <Engine/Application.h>

namespace gui
{
	guiTree_GameObject::guiTree_GameObject()
		: guiChild("guiTree_GameObject")
		, mTreeWidget(nullptr)
	{

		mh::int2 winSize = mh::Application::GetWindowSize();
		//SetSize(ImVec2((float)(winSize.x / 2), (float)(winSize.y / 2)));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetKey("Scenes");
		//AddChild(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&guiTree_GameObject::InitializeInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeScene();
	}

	guiTree_GameObject::~guiTree_GameObject()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}


	void guiTree_GameObject::Update()
	{
	}


	void guiTree_GameObject::InitializeInspector(tData _data)
	{
		mh::RenderMgr::SetInspectorGameObject(static_cast<mh::GameObject*>(_data.pData));

		guiInspector* inspector = ImGuiMgr::FindGUIWindow<guiInspector>("guiInspector");
		inspector->SetTargetGameObject(mh::RenderMgr::GetInspectorGameObject());
		inspector->InitializeTargetGameObject();
	}

	void guiTree_GameObject::InitializeScene()
	{
		mTreeWidget->Clear();

		mh::IScene* scene = mh::SceneMgr::GetActiveScene();
		std::string sceneName = scene->GetKey();

		TreeWidget::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, tData{}, true);

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

	void guiTree_GameObject::AddGameObject(TreeWidget::tNode* parent, mh::GameObject* gameObject)
	{
		std::string name(gameObject->GetName());

		tData data{};
		data.SetDataPtr(gameObject);

		TreeWidget::tNode* node = mTreeWidget->AddNode(parent, name, data);
	}

}
