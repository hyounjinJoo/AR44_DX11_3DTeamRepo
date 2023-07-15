#include "MeshRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"

namespace mh
{
	MeshRenderer::MeshRenderer()
		: BaseRenderer(eComponentType::MeshRenderer)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::Initalize()
	{
	}

	void MeshRenderer::Update()
	{
	}

	void MeshRenderer::FixedUpdate()
	{
	}

	void MeshRenderer::Render()
	{
		//GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		//GetMaterial()->Bind();
		//GetMesh()->BindBuffer();

		//GetMesh()->Render();

		//GetMaterial()->Clear();
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		GetMesh()->BindBuffer();
		GetMaterial()->Bind();

		//Animator* animator = GetOwner()->GetComponent<Animator>();
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
