#include "PCH_Engine.h"
#include "Com_Renderer_3DAnimMesh.h"

#include "GameObject.h"
#include "Skeleton.h"
#

namespace mh
{
	Com_Renderer_3DAnimMesh::Com_Renderer_3DAnimMesh()
	{
	}

	Com_Renderer_3DAnimMesh::~Com_Renderer_3DAnimMesh()
	{
	}

	void Com_Renderer_3DAnimMesh::Render()
	{
		if (false == IsRenderReady())
			return;

		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			Material* mtrl = GetCurrentMaterial(i);
			if (mtrl)
			{
				//메쉬 바인딩
				GetMesh()->BindBuffer(i);

				//재질에 애니메이션 정보 넣어주고 바인딩
				mtrl->SetAnim3D(true);
				mtrl->SetBoneCount(GetMesh()->GetSkeleton()->GetBoneCount());
				mtrl->BindData();

				// 사용할 메쉬 업데이트 및 렌더링
				GetMesh()->Render(i);
			}
		}
	}

	void Com_Renderer_3DAnimMesh::RenderEnd()
	{
		//순회돌면서 데이터를 되돌려줌
		UINT materialCount = GetMaterialCount();
		for (UINT i = 0; i < materialCount; ++i)
		{
			Material* mtrl = GetCurrentMaterial(i);
			if (mtrl)
			{
				mtrl->SetAnim3D(false);
				mtrl->SetBoneCount(0);
			}
		}
	}
}

