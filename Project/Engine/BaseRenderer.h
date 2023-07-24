#pragma once
#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"

namespace mh
{
	class BaseRenderer : public IComponent
	{
	public:
		BaseRenderer(enums::eComponentType _type);
		virtual ~BaseRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetMesh(const std::shared_ptr<Mesh> _mesh) { mMesh = _mesh; }
		void SetMaterial(const std::shared_ptr <GPU::Material> _shader) { mMaterial = _shader; }
		std::shared_ptr<Mesh> GetMesh() const { return mMesh; }
		std::shared_ptr <GPU::Material> GetMaterial() const { return mMaterial; }

	private:
		std::shared_ptr <Mesh> mMesh;
		std::shared_ptr <GPU::Material> mMaterial;
	};
}
