
#include "ClientPCH.h"

#include <Engine/MeshRenderer.h>
#include <Engine/GameResources.h>
#include <Engine/SpriteRenderer.h>
#include <Engine/GameResource.h>

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
		SetName("MeshRenderer");
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
			= std::string(mMesh->GetName().begin(), mMesh->GetName().end());
		std::string materialName
			= std::string(mMaterial->GetName().begin(), mMaterial->GetName().end());

		ImGui::Text("Mesh"); 
		ImGui::InputText("##MeshName", (char*)meshName.data()
			, meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("##MeshBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUI = editor.GetWidget<ListWidget>("ListWidget");
			listUI->SetState(eState::Active);
			

			//모든 메쉬의 리소스를 가져와야한다.
			std::vector<std::shared_ptr<mh::Mesh>> meshes 
				= mh::GameResources::Finds<mh::Mesh>();

			std::vector<std::string> name;
			for (auto mesh : meshes)
			{
				name.push_back(mesh->GetName());
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
			std::vector<std::shared_ptr<mh::GPU::Material>> materials
				= mh::GameResources::Finds<mh::GPU::Material>();

			std::vector<std::string> Name;
			for (auto material : materials)
			{
				Name.push_back(material->GetName());
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
		std::shared_ptr<mh::Mesh> mesh = mh::GameResources::Find<mh::Mesh>(_strKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::MeshRenderer>()->SetMesh(mesh);
	}

	void MeshRenderer::SetMaterial(const std::string& _strKey)
	{
		std::shared_ptr<mh::GPU::Material> material = mh::GameResources::Find<mh::GPU::Material>(_strKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::MeshRenderer>()->SetMaterial(material);
	}
}
