#include "PCH_Engine.h"

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

	void Com_Renderer_Mesh::Init()
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
		//GetOwner()->GetTransform().SetConstBuffer();

		//GetMaterial()->Bind();
		//GetMesh()->BindBuffer();

		//GetMesh()->Render();

		//GetMaterial()->Clear();
		GetOwner()->GetTransform().SetConstBuffer();

		GetMesh()->BindBuffer();
		GetMaterial(0)->Bind();

		//Com_Animator* animator = GetOwner()->GetComponent<Com_Animator>();
		//if (animator)
		//{
		//	animator->Binds();
		//}

		GetMesh()->Render();
		GetMaterial(0)->Clear();

		//if (animator)
		//{
		//	animator->Clear();
		//}
	}
}
