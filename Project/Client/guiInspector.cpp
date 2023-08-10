#include "PCH_Client.h"

#include "guiInspector.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <Engine/RenderMgr.h>

#include "guiTransform.h"
#include "guiCom_Renderer_Mesh.h"
#include "guiTexture.h"

namespace gui
{
	using namespace mh::define;
	guiInspector::guiInspector()
		: guiChild("guiInspector")
		, mTargetResource()
		, mTransform()
	{
		//SetSize(ImVec2(300.0f, 100.0f));
		
		mComponents.resize((UINT)eComponentType::END);
		mTargetGameObject = mh::RenderMgr::GetInspectorGameObject();

		//TODO: 여기 작동 안함
		mTransform = new gui::guiTransform();
		mTransform->SetKey("InspectorTransform");
		mTransform->SetTarget(mTargetGameObject);
		//AddWidget(mTransform);
		//
		//mComponents[(UINT)eComponentType::] = new gui::Com_Renderer_Mesh();
		//mComponents[(UINT)eComponentType::Com_Renderer_Mesh]->SetKey("InspectorMeshRenderer");
		//mComponents[(UINT)eComponentType::Com_Renderer_Mesh]->SetTarget(mTargetGameObject);
		//AddWidget(mComponents[(UINT)eComponentType::Com_Renderer_Mesh]);

		mResources.resize((UINT)eResourceType::End);
		//mResources[(UINT)eResourceType::Texture] = new gui::Texture();
		mResources[(UINT)eResourceType::Texture]->SetKey("InspectorTexture");
		//AddWidget(mResources[(UINT)eResourceType::Texture]);
	}

	guiInspector::~guiInspector()
	{
		for (guiComponent* comp : mComponents)
		{
			delete comp;
			comp = nullptr;
		}

		for (gui::IRes* res : mResources)
		{
			delete res;
			res = nullptr;
		}
	}

	void guiInspector::Update()
	{
		//mComponents[(UINT)eComponentType::Transform]->SetTarget(mTarget);
		//mComponents[(UINT)eComponentType::Com_Renderer_Mesh]->SetTarget(mTarget);
	}


	void guiInspector::ClearTarget()
	{
		for (guiComponent* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->SetEnable(false);
			comp->SetTarget(nullptr);
		}

		for (gui::IRes* res : mResources)
		{
			if (res == nullptr)
				continue;

			res->SetEnable(false);
			res->SetTarget(nullptr);
		}
	}

	void guiInspector::InitializeTargetGameObject()
	{
		ClearTarget();



		//mComponents[(UINT)eComponentType::]->SetState(eState::Active);
		//mComponents[(UINT)eComponentType::Transform]->SetTarget(mTargetGameObject);
		//mComponents[(UINT)eComponentType::Com_Renderer_Mesh]->SetState(eState::Active);
		//mComponents[(UINT)eComponentType::Com_Renderer_Mesh]->SetTarget(mTargetGameObject);
	}

	void guiInspector::InitializeTargetResource()
	{
		ClearTarget();

		//mResources[(UINT)eResourceType::Texture]->SetState(eState::Active);
		//mResources[(UINT)eResourceType::Texture]->SetTarget(mTargetResource);
	}
}
