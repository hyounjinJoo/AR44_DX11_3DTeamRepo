#include "PCH_Engine.h"
#include "Com_Renderer_Sprite.h"

#include "Mesh.h"
#include "Material.h"

#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_Animator2D.h"

#include "ResMgr.h"

namespace mh
{
	Com_Renderer_Sprite::Com_Renderer_Sprite()
		: mAnimator()
	{
	}

	Com_Renderer_Sprite::~Com_Renderer_Sprite()
	{
	}

	void Com_Renderer_Sprite::Init()
	{
		//Renderer가 렌더링할 때 필요한 것들
		//1. 메쉬
		//2. 재질
		//	재질 1. 쉐이더
		//	재질 2. 텍스처

		//기본 Rect Mesh로 메쉬를 설정
		std::shared_ptr<Mesh> mesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
		SetMesh(mesh);

		//Sprite Material을 받아온다
		std::shared_ptr<Material> material = ResMgr::Find<Material>(strKey::Default::material::SpriteMaterial);
		SetMaterial(material, 0);

		mAnimator = GetOwner()->GetComponent<Com_Animator2D>();
	}


	void Com_Renderer_Sprite::FixedUpdate()
	{
		IAnimator* animator = static_cast<IAnimator*>(GetOwner()->GetComponent(eComponentType::Animator));
		if (nullptr == animator)
		{
			mAnimator = nullptr;
		}
		else if (nullptr == mAnimator && animator)
		{
			mAnimator = dynamic_cast<Com_Animator2D*>(animator);
		}
	}

	void Com_Renderer_Sprite::Render()
	{
		if (false == IsRenderReady() || nullptr == mAnimator)
			return;

		//재질 바인딩
		mAnimator->BindData();

		//메쉬 바인딩
		GetMesh()->BindBuffer();

		//재질 바인딩
		GetCurrentMaterial(0)->BindData();

		//렌더링 하고
		GetMesh()->Render();

		//재질 언바인딩
		GetCurrentMaterial(0)->UnBindData();

		//애니메이터 언바인드
		mAnimator->UnBindData();
	}
}
