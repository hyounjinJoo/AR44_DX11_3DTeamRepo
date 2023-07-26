#pragma once
#include "guiComponent.h"
#include <Engine/Material.h>
#include <Engine/Mesh.h>

namespace gui
{
	class guiCom_Renderer_Mesh : public guiComponent
	{
	public:
		guiCom_Renderer_Mesh();
		~guiCom_Renderer_Mesh();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	
		void SetMesh(const std::string& _strKey);
		void SetMaterial(const std::string& _strKey);

	private:
		std::shared_ptr<mh::GPU::Material> mMaterial;
		std::shared_ptr<mh::Mesh> mMesh;
	};
}
