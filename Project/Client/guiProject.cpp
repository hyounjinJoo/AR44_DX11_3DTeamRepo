
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
		: guiChild("Project")
		, mTreeWidget(nullptr)
	{

		mh::int2 winSize = mh::Application::GetWindowSize();

		Vector2 size((float)winSize.x, (float)winSize.y);

		//SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetKey("GameResources");
		
		AddChild(mTreeWidget);

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

	void Project::ResetContent()
	{
		//mTreeWidget->Close();
		mTreeWidget->Clear();

		TreeWidget::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "GameResources", tData{}, true);

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

	void Project::toInspector(tData _data)
	{
		mh::IRes* resource = static_cast<mh::IRes*>(_data.pData);

		guiInspector* inspector = ImGuiMgr::FindGUIWindow<guiInspector>("guiInspector");
		inspector->SetTargetResource(resource);
		inspector->InitializeTargetResource();
	}

}
