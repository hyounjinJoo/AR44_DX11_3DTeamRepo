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

	void IRenderer::Initialize()
	{
	}

	void IRenderer::Update()
	{
	}

	void IRenderer::FixedUpdate()
	{
	}

	void IRenderer::Render()
	{
	}

}
