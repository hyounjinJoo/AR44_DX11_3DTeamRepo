#pragma once
#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "BaseRenderer.h"


namespace mh
{
	using namespace mh::GPU;

	class MeshRenderer : public BaseRenderer
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
