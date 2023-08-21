#include "PCH_Engine.h"
#include "Com_Renderer_MultiMesh.h"
#include "Com_Renderer_Mesh.h"

namespace mh
{
	Com_Renderer_MultiMesh::Com_Renderer_MultiMesh()
	{
	}
	Com_Renderer_MultiMesh::~Com_Renderer_MultiMesh()
	{
	}

	void Com_Renderer_MultiMesh::AddMeshRenderer(Com_Renderer_Mesh* _pMesh)
	{
		if (_pMesh)
		{
			mRenderers.push_back(_pMesh);
			_pMesh->SetOwner(GetOwner());
		}
	}

	define::eRenderingMode Com_Renderer_MultiMesh::GetRenderingMode()
	{
		eRenderingMode mode = eRenderingMode::None;
		if (false == mRenderers.empty() && mRenderers[0])
		{
			mode = mRenderers[0]->GetRenderingMode();
		}
		return mode;
	}

	void Com_Renderer_MultiMesh::Render()
	{
		for (size_t i = 0; i < mRenderers.size(); ++i)
		{
			if (mRenderers[i])
				mRenderers[i]->Render();
		}
	}
}
