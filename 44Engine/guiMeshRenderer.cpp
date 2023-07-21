
#include "ClientPCH.h"

#include "guiMeshRenderer.h"
#include "MeshRenderer.h"
#include "guiEditor.h"
#include "guiListWidget.h"
#include "GameResources.h"
#include "GameResource.h"
#include "guiInspector.h"
#include "SpriteRenderer.h"

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

			std::vector<std::wstring> name;
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
			std::vector<std::shared_ptr<mh::graphics::Material>> materials
				= mh::GameResources::Finds<mh::graphics::Material>();

			std::vector<std::wstring> wName;
			for (auto material : materials)
			{
				wName.push_back(material->GetName());
			}

			listUI->SetItemList(wName);
			listUI->SetEvent(this, std::bind(&MeshRenderer::SetMaterial
				, this, std::placeholders::_1));
		}
	}

	void MeshRenderer::LateUpdate()
	{
		IComponent::LateUpdate();
	}

	void MeshRenderer::SetMesh(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		std::shared_ptr<mh::Mesh> mesh = mh::GameResources::Find<mh::Mesh>(wKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::MeshRenderer>()->SetMesh(mesh);
	}

	void MeshRenderer::SetMaterial(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		std::shared_ptr<mh::graphics::Material> material = mh::GameResources::Find<mh::graphics::Material>(wKey);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->GetTargetGameObject()->GetComponent<mh::MeshRenderer>()->SetMaterial(material);
	}
}
