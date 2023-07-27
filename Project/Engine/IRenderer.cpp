#include "EnginePCH.h"

#include "IRenderer.h"

namespace mh
{
	IRenderer::IRenderer()
		: IComponent(define::eComponentType::Renderer)
		, mMesh(nullptr)
		, mMaterial(nullptr)
	{
	}

	IRenderer::~IRenderer()
	{
	}

}



