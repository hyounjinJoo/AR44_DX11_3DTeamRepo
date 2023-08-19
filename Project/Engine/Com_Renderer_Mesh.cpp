#include "PCH_Engine.h"

#include "Com_Renderer_Mesh.h"
#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_Animator2D.h"
#include "Com_Animator3D.h"

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
		//메쉬 또는 재질이 없을 경우 렌더링 불가능이므로 return;
		if (nullptr == GetMesh() || nullptr == GetMaterial(0))
			return;

		//애니메이션 확인
		IAnimator* Animator = static_cast<IAnimator*>(GetOwner()->GetComponent(define::eComponentType::Animator));
		if (Animator)
		{
			define::eDimensionType animDimType = Animator->GetDimensionType();

			Animator->Binds();

			if(define::eDimensionType::_3D == animDimType)
			{
				Com_Animator3D* Animator3D = static_cast<Com_Animator3D*>(Animator);

				UINT materialCount = GetMaterialCount();

				for (UINT i = 0; i < materialCount; ++i)
				{
					std::shared_ptr<Material> mtrl = GetMaterial(i);
					if (mtrl)
					{
						mtrl->SetAnim3D(true);
						mtrl->SetBoneCount(Animator3D->GetBoneCount());
					}
				}
			}
		}
			
		
		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			if (nullptr != GetMaterial(i))
			{
				GetMesh()->BindBuffer(i);

				// 사용할 재질 업데이트
				GetMaterial(i)->Bind();

				// 사용할 메쉬 업데이트 및 렌더링
				GetMesh()->Render(i);
			}
		}

		if (Animator)
		{
			Animator->Clear();
		}
	}
}
