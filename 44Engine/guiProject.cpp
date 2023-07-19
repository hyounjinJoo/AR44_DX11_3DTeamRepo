#include "guiProject.h"

#include "Texture.h"
#include "Material.h"
#include "yaMesh.h"
#include "yaShader.h"

#include "guiInspector.h"
#include "guiResource.h"
#include "Resources.h"
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
		mTreeWidget->SetName("Resources");
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

		TreeWidget::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "Resources", 0, true);

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
		AddResources<ya::Mesh>(pRootNode, "Mesh");
		AddResources<ya::graphics::Texture>(pRootNode, "Texture");
		AddResources<ya::graphics::Material>(pRootNode, "Materials");
		AddResources<ya::Shader>(pRootNode, "Shaders");
	}

	void Project::toInspector(void* data)
	{
		ya::Resource* resource = static_cast<ya::Resource*>(data);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->SetTargetResource(resource);
		inspector->InitializeTargetResource();
	}

}