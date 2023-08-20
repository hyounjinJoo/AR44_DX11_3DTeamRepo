#include "PCH_Engine.h"

#include "Com_Renderer_Sprite.h"
#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_Animator2D.h"

#include "ResMgr.h"

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
		//Renderer가 렌더링할 때 필요한 것들
		//1. 메쉬
		//2. 재질
		//	재질 1. 쉐이더
		//	재질 2. 텍스처

		//기본 Rect Mesh로 메쉬를 설정
		std::shared_ptr<Mesh> mesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
		SetMesh(mesh);

		//Material을 생성(임시)
		std::shared_ptr<Material> material = std::make_shared<Material>();
		
		//스프라이트 쉐이더를 찾아서 material에 적용
		std::shared_ptr<GraphicsShader> spriteShader = ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::SpriteShader);
		
		//쉐이더를 지정
		material->SetShader(spriteShader);
		
		//텍스처는 좀이따 렌더링할때 지정해주면 됨
		SetMaterial(material, 0u);
	}

	void Com_Renderer_Sprite::Update()
	{
	}

	void Com_Renderer_Sprite::FixedUpdate()
	{
	}

	void Com_Renderer_Sprite::Render()
	{
		//GetOwner()->GetComponent<Com_Transform>()->BindData();

		GetMesh()->BindBuffer();
		GetCurrentMaterial(0)->BindData();

		Com_Animator2D* animator = GetOwner()->GetComponent<Com_Animator2D>();
		if (animator)
		{
			animator->BindData();
		}

		GetMesh()->Render();
		GetCurrentMaterial(0)->UnBindData();

		if (animator)
		{
			animator->UnBindData();
		}
	}
}
