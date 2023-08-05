
#include "PCH_Client.h"

#include <Engine/Com_Renderer_Mesh.h>
#include <Engine/ResMgr.h>
#include <Engine/Com_Renderer_Sprite.h>
#include <Engine/IRes.h>

#include "guiCom_Renderer_Mesh.h"
#include "guiEditor.h"
#include "guiListWidget.h"
#include "guiInspector.h"


extern gui::EditorMgr gEditor;

namespace gui
{
	guiCom_Renderer_Mesh::guiCom_Renderer_Mesh()
		: guiComponent(eComponentType::Renderer)
	{
		SetKey("guiCom_Renderer_Mesh");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	guiCom_Renderer_Mesh::~guiCom_Renderer_Mesh()
	{

	}

	void guiCom_Renderer_Mesh::FixedUpdate()
	{
		guiComponent::FixedUpdate();

		if (GetTarget())
		{
			mh::Com_Renderer_Mesh* meshRenderer
				= GetTarget()->GetComponent<mh::Com_Renderer_Mesh>();

			if (meshRenderer == nullptr)
				return;

			//mh::SpriteRenderer* spriteRenderer
			//	= GetTarget()->GetComponent<mh::SpriteRenderer>();

			//if (spriteRenderer == nullptr)
			//	return;


			mMaterial = meshRenderer->GetMaterial();
			mMesh = meshRenderer->GetMesh();
		}
	}

	void guiCom_Renderer_Mesh::Update()
	{
		guiComponent::Update();

		if (mMesh == nullptr
			|| mMaterial == nullptr)
			return;

		std::string meshName
			= mMesh->GetKey();
		std::string materialName
			= mMaterial->GetKey();

		ImGui::Text("Mesh"); 
		ImGui::InputText("##MeshName", (char*)meshName.data()
			, meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("##MeshBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUI = gEditor.GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			

			//모든 메쉬의 리소스를 가져와야한다.
			const auto& meshes 
				= mh::ResMgr::GetResources<mh::Mesh>();

			std::vector<std::string> name;
			for (const auto& mesh : meshes)
			{
				name.push_back(mesh.first);
			}

			listUI->SetItemList(name);
			listUI->SetEvent(this, std::bind(&guiCom_Renderer_Mesh::SetMesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUI = gEditor.GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			//모든 메쉬의 리소스를 가져와야한다.
			const auto& materials
				= mh::ResMgr::GetResources<mh::Material>();

			std::vector<std::string> Name;
			for (const auto& material : materials)
			{
				Name.push_back(material.first);
			}

			listUI->SetItemList(Name);
			listUI->SetEvent(this, std::bind(&guiCom_Renderer_Mesh::SetMaterial
				, this, std::placeholders::_1));
		}
	}

	void guiCom_Renderer_Mesh::LateUpdate()
	{
		guiComponent::LateUpdate();
	}

	void guiCom_Renderer_Mesh::SetMesh(const std::string& _strKey)
	{
		std::shared_ptr<mh::Mesh> mesh = mh::ResMgr::Find<mh::Mesh>(_strKey);

		Inspector* inspector = gEditor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::Com_Renderer_Mesh>()->SetMesh(mesh);
	}

	void guiCom_Renderer_Mesh::SetMaterial(const std::string& _strKey)
	{
		std::shared_ptr<mh::Material> material = mh::ResMgr::Find<mh::Material>(_strKey);

		Inspector* inspector = gEditor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::Com_Renderer_Mesh>()->SetMaterial(material);
	}
}
