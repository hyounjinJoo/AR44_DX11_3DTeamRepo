#include "PCH_Engine.h"
#include "IRenderer.h"

namespace mh
{
	IRenderer::IRenderer()
		: IComponent(define::eComponentType::Renderer)
	{
	}

	IRenderer::~IRenderer()
	{
	}


}



