#include "EnginePCH.h"

#include "Com_Renderer_Sprite.h"
#include "GameObject.h"
#include "Transform.h"
#include "Com_Animator.h"

namespace mh
{
	Com_Renderer_Sprite::Com_Renderer_Sprite()
	{
	}

	Com_Renderer_Sprite::~Com_Renderer_Sprite()
	{
	}

	void Com_Renderer_Sprite::Initialize()
	{
	}

	void Com_Renderer_Sprite::Update()
	{
	}

	void Com_Renderer_Sprite::FixedUpdate()
	{
	}

	void Com_Renderer_Sprite::Render()
	{
		GetOwner()->GetTransform().SetConstantBuffer();

		GetMesh()->BindBuffer();
		GetMaterial()->Bind();

		Com_Animator* animator = GetOwner()->GetComponent<Com_Animator>();
		if (animator)
		{
			animator->Binds();
		}

		GetMesh()->Render();
		GetMaterial()->Clear();

		if (animator)
		{
			animator->Clear();
		}
	}
}
