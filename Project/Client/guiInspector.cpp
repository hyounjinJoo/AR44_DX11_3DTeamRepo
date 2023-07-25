#include "ClientPCH.h"

#include "guiInspector.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <Engine/Renderer.h>

#include "guiTransform.h"
#include "guiMeshRenderer.h"
#include "guiTexture.h"

namespace gui
{
	using namespace mh::define;
	Inspector::Inspector()
	{
		SetKey("Inspector");
		SetSize(ImVec2(300.0f, 100.0f));
		
		mComponents.resize((UINT)eComponentType::END);
		mTargetGameObject = mh::renderer::gInspectorGameObject;

		//TODO: 여기 작동 안함
		//mTransform = new gui::Transform();
		//mTransform->SetKey("InspectorTransform");
		//mTransform->SetTarget(mTargetGameObject);
		//AddWidget(mTransform);
		//
		//mComponents[(UINT)eComponentType::] = new gui::MeshRenderer();
		//mComponents[(UINT)eComponentType::MeshRenderer]->SetKey("InspectorMeshRenderer");
		//mComponents[(UINT)eComponentType::MeshRenderer]->SetTarget(mTargetGameObject);
		//AddWidget(mComponents[(UINT)eComponentType::MeshRenderer]);

		mResources.resize((UINT)eResourceType::End);
		mResources[(UINT)eResourceType::Texture] = new gui::Texture();
		mResources[(UINT)eResourceType::Texture]->SetKey("InspectorTexture");
		AddWidget(mResources[(UINT)eResourceType::Texture]);
	}

	Inspector::~Inspector()
	{
		for (gui::IComponent* comp : mComponents)
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

	void Inspector::FixedUpdate()
	{
		//mComponents[(UINT)eComponentType::Transform]->SetTarget(mTarget);
		//mComponents[(UINT)eComponentType::MeshRenderer]->SetTarget(mTarget);
	}

	void Inspector::Update()
	{
		
	}

	void Inspector::LateUpdate()
	{
	}

	void Inspector::ClearTarget()
	{
		for (gui::IComponent* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->SetState(eState::Paused);
			comp->SetTarget(nullptr);
		}

		for (gui::IRes* res : mResources)
		{
			if (res == nullptr)
				continue;

			res->SetState(eState::Paused);
			res->SetTarget(nullptr);
		}
	}

	void Inspector::InitializeTargetGameObject()
	{
		ClearTarget();



		//mComponents[(UINT)eComponentType::]->SetState(eState::Active);
		//mComponents[(UINT)eComponentType::Transform]->SetTarget(mTargetGameObject);
		//mComponents[(UINT)eComponentType::MeshRenderer]->SetState(eState::Active);
		//mComponents[(UINT)eComponentType::MeshRenderer]->SetTarget(mTargetGameObject);
	}

	void Inspector::InitializeTargetResource()
	{
		ClearTarget();

		mResources[(UINT)eResourceType::Texture]->SetState(eState::Active);
		mResources[(UINT)eResourceType::Texture]->SetTarget(mTargetResource);
	}
}
