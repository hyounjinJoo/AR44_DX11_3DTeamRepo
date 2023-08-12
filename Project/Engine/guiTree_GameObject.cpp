#include "PCH_Engine.h"
#include "guiTree_GameObject.h"

#include <Engine/Application.h>
#include <Engine/IScene.h>
#include <Engine/Layer.h>
#include <Engine/SceneMgr.h>
#include <Engine/RenderMgr.h>

#include "guiInspector.h"
#include "guiMgr.h"
#include "guiInspector.h"

#include <Engine/Application.h>

namespace gui
{
	guiTree_GameObject::guiTree_GameObject()
		: guiWindow(strKey::GameObjects)
		, mTreeWidget(nullptr)
	{

		mh::int2 winSize = mh::Application::GetWindowSize();
		//SetSize(ImVec2((float)(winSize.x / 2), (float)(winSize.y / 2)));

		mTreeWidget = AddChild<TreeWidget>();
		//AddChild(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&guiTree_GameObject::GameobjectSelectCallback, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeScene();
	}

	guiTree_GameObject::~guiTree_GameObject()
	{
	}


	void guiTree_GameObject::Update()
	{
	}


	void guiTree_GameObject::GameobjectSelectCallback(mh::define::tDataPtr _data)
	{
		mh::RenderMgr::SetInspectorGameObject(static_cast<mh::GameObject*>(_data.pData));
		guiInspector* inspector = guiMgr::FindGuiWindow<guiInspector>(strKey::Inspector);
		inspector->SetTargetGameObject(mh::RenderMgr::GetInspectorGameObject());
	}

	void guiTree_GameObject::InitializeScene()
	{
		mTreeWidget->Clear();

		mh::IScene* scene = mh::SceneMgr::GetActiveScene();
		std::string sceneName = scene->GetKey();

		TreeWidget::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, mh::define::tDataPtr{}, true);

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

		mh::define::tDataPtr data{};
		data.SetDataPtr(gameObject);

		TreeWidget::tNode* node = mTreeWidget->AddNode(parent, name, data);
	}

}
