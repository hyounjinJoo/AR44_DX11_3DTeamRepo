#pragma once
#include "IRenderer.h"


namespace mh
{
	class Com_Renderer_Mesh;
	class Com_Renderer_MultiMesh :
		public IRenderer
	{
	public:
		Com_Renderer_MultiMesh();
		virtual ~Com_Renderer_MultiMesh();

		virtual void FixedUpdate() override {};
		virtual void Render() override;
		
		void AddMeshRenderer(Com_Renderer_Mesh* _pMesh);

		virtual define::eRenderingMode GetRenderingMode() override;

	private:
		std::vector<Com_Renderer_Mesh*> mRenderers;
	};
}


