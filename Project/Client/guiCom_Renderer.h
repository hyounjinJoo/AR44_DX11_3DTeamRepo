#pragma once
#include "guiComponent.h"
#include <Engine/Material.h>
#include <Engine/Mesh.h>

namespace gui
{
	class guiCom_Renderer : public guiComponent
	{
	public:
		guiCom_Renderer();
		virtual ~guiCom_Renderer();

		virtual void Update() override;

		virtual void UpdateUI() override;
	
		void SetMesh(const std::string& _strKey);
		void SetMaterial(const std::string& _strKey);

	private:
		std::shared_ptr<mh::Material> mMaterial;
		std::shared_ptr<mh::Mesh> mMesh;
	};
}
