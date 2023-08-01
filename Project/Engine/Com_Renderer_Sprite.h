#pragma once

#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "IRenderer.h"

using namespace mh;
namespace mh
{
	class Com_Renderer_Sprite : public IRenderer
	{
	public:
		Com_Renderer_Sprite();
		virtual ~Com_Renderer_Sprite();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
