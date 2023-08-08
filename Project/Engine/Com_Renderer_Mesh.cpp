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

		for (UINT i = 0u; i < GetMesh()->GetSubsetCount(); ++i)
		{
			GetMesh()->BindBuffer(i);
			GetMaterial(i)->Bind();

			GetMesh()->Render(i);

			//Com_Animator* animator = GetOwner()->GetComponent<Com_Animator>();
			//if (animator)
			//{
			//	animator->Binds();
			//}

			GetMaterial(i)->Clear();

			//if (animator)
			//{
			//	animator->Clear();
			//}
		}


	}
}
