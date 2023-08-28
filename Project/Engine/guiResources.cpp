
#include "PCH_Engine.h"

#include "guiResources.h"

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "GraphicsShader.h"
#include "ResMgr.h"

#include "guiInspector.h"
#include "guiResource.h"

#include "guiMgr.h"

#include "Application.h"

namespace gui
{
	using namespace mh::math;

	guiResources::guiResources()
		: guiWindow(strKey::ResourceViewer)
		, mTreeWidget(nullptr)
	{
		mTreeWidget = AddChild<TreeWidget>();

		mTreeWidget->SetEvent(this
			, std::bind(&guiResources::toInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);
	}

	guiResources::~guiResources()
	{
	}

	void guiResources::Init()
	{
		ResetContent();
	}

	void guiResources::ResetContent()
	{
		//mTreeWidget->Close();
		mTreeWidget->Clear();

		TreeWidget::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "GameResources", mh::define::tDataPtr{}, true);

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

	void guiResources::toInspector(mh::define::tDataPtr _data)
	{
		mh::IRes* resource = static_cast<mh::IRes*>(_data.pData);

		guiInspector* inspector = guiMgr::FindGuiWindow<guiInspector>(strKey::Inspector);
		inspector->SetTargetResource(resource);
		//inspector->InitializeTargetResource();
	}

}
