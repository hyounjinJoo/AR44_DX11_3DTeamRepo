#pragma once

#include "guiWidget.h"

namespace mh
{
	class GraphicsShader;
}

namespace gui
{
	class GraphicsShaderEditor
		: public Widget
	{
	public:
		GraphicsShaderEditor();
		virtual ~GraphicsShaderEditor();

		//virtual void FixedUpdate();
		virtual void Update();
		//virtual void LateUpdate();
		//virtual void Render();
		virtual void Close();

		bool CreateDefaultShaders();


	};
}


