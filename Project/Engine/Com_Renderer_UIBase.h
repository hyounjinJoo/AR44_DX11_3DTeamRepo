#pragma once
#include "Com_Renderer_Mesh.h"

namespace mh
{
	class Com_Renderer_UIBase :
		public Com_Renderer_Mesh
	{
	public:
		Com_Renderer_UIBase();
		virtual ~Com_Renderer_UIBase();

		virtual void Render() override;

		tCB_UniformData& GetUIUniformData() { return mUIData; }

	private:
		tCB_UniformData mUIData;
	};
}


