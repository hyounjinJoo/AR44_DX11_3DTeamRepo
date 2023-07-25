#pragma once
#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "IRenderer.h"


namespace mh
{
	using namespace mh::GPU;

	class Com_Renderer_Mesh : public IRenderer
	{
	public:
		Com_Renderer_Mesh();
		virtual ~Com_Renderer_Mesh();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
