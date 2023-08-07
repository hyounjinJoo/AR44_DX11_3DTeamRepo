
#include "PCH_Client.h"

#include "guiProject.h"

#include <Engine/Texture.h>
#include <Engine/Material.h>
#include <Engine/Mesh.h>
#include <Engine/GraphicsShader.h>
#include <Engine/ResMgr.h>

#include "guiInspector.h"
#include "guiResource.h"

#include "ImGuiMgr.h"

#include <Engine/Application.h>

namespace gui
{
	using namespace mh::math;

	Project::Project()
		: mTreeWidget(nullptr)
	{
		SetKey("Project");

		mh::int2 winSize = mh::Application::GetWIndowSize();

		Vector2 size((float)winSize.x, (float)winSize.y);

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetKey("GameResources");
		AddWidget(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&Project::toInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);
		ResetContent();
	}

	Project::~Project()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}

	void Project::FixedUpdate()
	{
		Widget::FixedUpdate();

		//리소스가 바뀐다면 리소스목록 초기화
	}

	void Project::Update()
	{
		Widget::Update();
	}

	void Project::LateUpdate()
	{

	}

	void Project::ResetContent()
	{
		//mTreeWidget->Close();
		mTreeWidget->Clear();

		TreeWidget::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "GameResources", 0, true);

		//enum class eResourceType
		//{
		//	Mesh,
		//	Texture,
		//	Material,
		//	Sound,
		//	Prefab,
		//	MeshData,
		//	GraphicsShader,
		//	ComputeShader,
		//	End,
		//};
		AddResources<mh::Mesh>(pRootNode, "Mesh");
		AddResources<mh::Texture>(pRootNode, "Texture");
		AddResources<mh::Material>(pRootNode, "Materials");
		AddResources<mh::GraphicsShader>(pRootNode, "Shaders");
	}

	void Project::toInspector(void* data)
	{
		mh::IRes* resource = static_cast<mh::IRes*>(data);

		Inspector* inspector = ImGuiMgr::GetWidget<Inspector>("Inspector");
		inspector->SetTargetResource(resource);
		inspector->InitializeTargetResource();
	}

}
