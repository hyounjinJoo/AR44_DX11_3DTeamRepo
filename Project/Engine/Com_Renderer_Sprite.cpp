#include "PCH_Engine.h"

#include "Com_Renderer_Sprite.h"
#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_Animator.h"

namespace mh
{
	Com_Renderer_Sprite::Com_Renderer_Sprite()
	{
	}

	Com_Renderer_Sprite::~Com_Renderer_Sprite()
	{
	}

	void Com_Renderer_Sprite::Init()
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
		GetOwner()->GetTransform().SetConstBuffer();

		GetMesh()->BindBuffer();
		GetMaterial(0)->Bind();

		Com_Animator* animator = GetOwner()->GetComponent<Com_Animator>();
		if (animator)
		{
			animator->Binds();
		}

		GetMesh()->Render();
		GetMaterial(0)->Clear();

		if (animator)
		{
			animator->Clear();
		}
	}
}
