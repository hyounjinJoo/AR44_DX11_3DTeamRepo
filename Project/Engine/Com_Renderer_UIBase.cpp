#include "PCH_Engine.h"
#include "Com_Renderer_UIBase.h"

#include "RenderMgr.h"
#include "ConstBuffer.h"

#include "ResMgr.h"
#include "Mesh.h"

namespace mh
{
	Com_Renderer_UIBase::Com_Renderer_UIBase()
		: mUIData()
	{
		//기본 UI Mesh, Material을 설정
		std::shared_ptr<Mesh> uiMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
		SetMesh(uiMesh);

		std::shared_ptr<Material> uiMaterial = ResMgr::Find<Material>(strKey::Default::material::UIMaterial);
		SetMaterial(uiMaterial, 0u);
		SetMaterialMode(0u, eMaterialMode::Dynamic);
	}
	Com_Renderer_UIBase::~Com_Renderer_UIBase()
	{
	}


	void Com_Renderer_UIBase::Render()
	{
		ConstBuffer* cb = RenderMgr::GetConstBuffer(define::eCBType::UniformData);
		cb->SetData(&mUIData);
		cb->BindData(eShaderStageFlag::VS | eShaderStageFlag::PS);

		Com_Renderer_Mesh::Render();
	}
}

