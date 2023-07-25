#include "EnginePCH.h"

#include "BaseRenderer.h"

namespace mh
{
	BaseRenderer::BaseRenderer(define::eComponentType _type)
		: IComponent(_type)
		, mMesh(nullptr)
		, mMaterial(nullptr)
	{

	}

	BaseRenderer::~BaseRenderer()
	{
	}

	void BaseRenderer::Initialize()
	{
	}

	void BaseRenderer::Update()
	{
	}

	void BaseRenderer::FixedUpdate()
	{
	}

	void BaseRenderer::Render()
	{
	}

}
