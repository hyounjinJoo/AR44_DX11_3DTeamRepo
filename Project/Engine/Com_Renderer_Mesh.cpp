#include "EnginePCH.h"

#include "Com_Renderer_Mesh.h"
#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_Animator.h"

namespace mh
{
	Com_Renderer_Mesh::Com_Renderer_Mesh()
	{
	}

	Com_Renderer_Mesh::~Com_Renderer_Mesh()
	{
	}

	void Com_Renderer_Mesh::Initialize()
	{
	}

	void Com_Renderer_Mesh::Update()
	{
	}

	void Com_Renderer_Mesh::FixedUpdate()
	{
	}

	void Com_Renderer_Mesh::Render()
	{
		//GetOwner()->GetTransform().SetConstantBuffer();

		//GetMaterial()->Bind();
		//GetMesh()->BindBuffer();

		//GetMesh()->Render();

		//GetMaterial()->Clear();
		GetOwner()->GetTransform().SetConstantBuffer();

		GetMesh()->BindBuffer();
		GetMaterial()->Bind();

		//Com_Animator* animator = GetOwner()->GetComponent<Com_Animator>();
		//if (animator)
		//{
		//	animator->Binds();
		//}

		GetMesh()->Render();
		GetMaterial()->Clear();

		//if (animator)
		//{
		//	animator->Clear();
		//}
	}
}
