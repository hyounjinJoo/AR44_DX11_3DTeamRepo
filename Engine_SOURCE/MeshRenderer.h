#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"
#include "BaseRenderer.h"

using namespace mh::graphics;
namespace mh
{
	class MeshRenderer : public BaseRenderer
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}