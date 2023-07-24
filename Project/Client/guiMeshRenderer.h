#pragma once
#include "guiComponent.h"
#include <Engine/Material.h>
#include <Engine/Mesh.h>

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
		std::shared_ptr<mh::graphics::Material> mMaterial;
		std::shared_ptr<mh::Mesh> mMesh;
	};
}
