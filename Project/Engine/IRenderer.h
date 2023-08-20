#pragma once
#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"

namespace mh
{
	enum class eMaterialMode
	{
		Shared,
		Dynamic
	};
	struct tMaterialSet
	{
		std::shared_ptr<Material>	SharedMaterial;
		std::unique_ptr<Material>	DynamicMaterial;
		Material*					CurrentMaterial;

		eMaterialMode MaterialMode;
	};

	class IRenderer : public IComponent
	{
	public:
		IRenderer();

		IRenderer(const IRenderer& _other) = default;

		virtual ~IRenderer();

		virtual void Render() = 0;

		virtual define::eRenderingMode GetRenderingMode() = 0;
	};

}
