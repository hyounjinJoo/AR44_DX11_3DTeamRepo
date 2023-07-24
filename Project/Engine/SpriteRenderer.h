#pragma once

#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "BaseRenderer.h"

using namespace mh::GPU;
namespace mh
{
	class SpriteRenderer : public BaseRenderer
	{
	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
