#pragma once

#include "guiWidget.h"
#include <Engine/define_GPU.h>

namespace mh
{
	class GraphicsShader;
}

namespace gui
{
	class GraphicsShaderEditor
		: public guiWidget
	{
	public:
		GraphicsShaderEditor();
		virtual ~GraphicsShaderEditor();

		//virtual void FixedUpdate();
		virtual void Update();
		//virtual void LateUpdate();
		//virtual void Render();

		bool CreateDefaultShaders();

		 
	private:
		std::vector<std::string> mSemanticNames;
		std::vector<D3D11_INPUT_ELEMENT_DESC> mInputElements;


		std::unique_ptr<mh::GraphicsShader> mEditTarget;

	};
}


