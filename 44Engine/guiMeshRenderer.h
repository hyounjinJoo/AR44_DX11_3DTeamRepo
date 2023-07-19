#pragma once
#include "guiComponent.h"
#include "Material.h"
#include "yaMesh.h"

namespace gui
{
	class MeshRenderer : public IComponent
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	
		void SetMesh(std::string key);
		void SetMaterial(std::string key);

	private:
		std::shared_ptr<ya::graphics::Material> mMaterial;
		std::shared_ptr<ya::Mesh> mMesh;
	};
}