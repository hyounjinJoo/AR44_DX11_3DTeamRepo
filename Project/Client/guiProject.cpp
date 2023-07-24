
#include "ClientPCH.h"

#include "guiProject.h"

#include <Engine/Texture.h>
#include <Engine/Material.h>
#include <Engine/Mesh.h>
#include <Engine/Shader.h>
#include <Engine/GameResources.h>

#include "guiInspector.h"
#include "guiResource.h"

#include "guiEditor.h"

extern gui::Editor editor;

namespace gui
{
	Project::Project()
		: mTreeWidget(nullptr)
	{
		SetName("Project");
		UINT width = 1600;
		UINT height = 900;

		Vector2 size(width, height);

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("GameResources");
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
		AddResources<mh::graphics::Texture>(pRootNode, "Texture");
		AddResources<mh::graphics::Material>(pRootNode, "Materials");
		AddResources<mh::Shader>(pRootNode, "Shaders");
	}

	void Project::toInspector(void* data)
	{
		mh::GameResource* resource = static_cast<mh::GameResource*>(data);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->SetTargetResource(resource);
		inspector->InitializeTargetResource();
	}

}
