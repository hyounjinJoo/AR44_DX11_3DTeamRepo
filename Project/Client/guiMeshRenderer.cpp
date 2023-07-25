
#include "ClientPCH.h"

#include <Engine/MeshRenderer.h>
#include <Engine/ResMgr.h>
#include <Engine/SpriteRenderer.h>
#include <Engine/IRes.h>

#include "guiMeshRenderer.h"
#include "guiEditor.h"
#include "guiListWidget.h"
#include "guiInspector.h"


extern gui::Editor editor;

namespace gui
{
	MeshRenderer::MeshRenderer()
		: IComponent(eComponentType::MeshRenderer)
	{
		SetKey("MeshRenderer");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::FixedUpdate()
	{
		IComponent::FixedUpdate();

		if (GetTarget())
		{
			mh::MeshRenderer* meshRenderer
				= GetTarget()->GetComponent<mh::MeshRenderer>();

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

	void MeshRenderer::Update()
	{
		IComponent::Update();

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
			ListWidget* listUI = editor.GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			

			//모든 메쉬의 리소스를 가져와야한다.
			const auto& meshes 
				= mh::ResMgr::GetInst()->GetResources<mh::Mesh>();

			std::vector<std::string> name;
			for (const auto& mesh : meshes)
			{
				name.push_back(mesh.first);
			}

			listUI->SetItemList(name);
			listUI->SetEvent(this, std::bind(&MeshRenderer::SetMesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUI = editor.GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			//모든 메쉬의 리소스를 가져와야한다.
			const auto& materials
				= mh::ResMgr::GetInst()->GetResources<mh::GPU::Material>();

			std::vector<std::string> Name;
			for (const auto& material : materials)
			{
				Name.push_back(material.first);
			}

			listUI->SetItemList(Name);
			listUI->SetEvent(this, std::bind(&MeshRenderer::SetMaterial
				, this, std::placeholders::_1));
		}
	}

	void MeshRenderer::LateUpdate()
	{
		IComponent::LateUpdate();
	}

	void MeshRenderer::SetMesh(const std::string& _strKey)
	{
		std::shared_ptr<mh::Mesh> mesh = mh::ResMgr::GetInst()->Find<mh::Mesh>(_strKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::MeshRenderer>()->SetMesh(mesh);
	}

	void MeshRenderer::SetMaterial(const std::string& _strKey)
	{
		std::shared_ptr<mh::GPU::Material> material = mh::ResMgr::GetInst()->Find<mh::GPU::Material>(_strKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::MeshRenderer>()->SetMaterial(material);
	}
}
