#pragma once

#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "IRenderer.h"

using namespace mh::GPU;
namespace mh
{
	class Com_Renderer_Sprite : public IRenderer
	{
	public:
		Com_Renderer_Sprite();
		virtual ~Com_Renderer_Sprite();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
