#pragma once
#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"

namespace mh
{
	struct tMaterialSet
	{
		std::shared_ptr<Material> SharedMaterial;
		std::shared_ptr<Material> DynamicMaterial;
		std::shared_ptr<Material> CurrentMaterial;
	};

	class IRenderer : public IComponent
	{
	public:
		IRenderer();

		IRenderer(const IRenderer& _other) = default;

		virtual ~IRenderer();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;


		//virtual void Init() override {};
		//virtual void Update() {};
		//virtual void FixedUpdate() override;
		virtual void Render() = 0;

		void SetMesh(const std::shared_ptr<Mesh> _mesh);
		inline void SetMaterial(const std::shared_ptr <Material> _Mtrl, uint _idx);
		std::shared_ptr<Mesh> GetMesh() const { return mMesh; }
		std::shared_ptr<Material> GetMaterial(uint _idx);
		
		
	private:
		std::shared_ptr <Mesh> mMesh;
		std::vector<tMaterialSet> mMaterials;

		//std::shared_ptr <Material> mMaterial;
	};

		inline void IRenderer::SetMaterial(const std::shared_ptr<Material> _Mtrl, uint _idx)
	{
		MH_ASSERT(mMaterials.size() > _idx);

		mMaterials[_idx].SharedMaterial = _Mtrl;
		mMaterials[_idx].CurrentMaterial = _Mtrl;
	}

	inline std::shared_ptr<Material> IRenderer::GetMaterial(uint _idx)
	{
		if (nullptr == mMaterials[_idx].CurrentMaterial)
		{
			mMaterials[_idx].CurrentMaterial = mMaterials[_idx].SharedMaterial;
		}

		return mMaterials[_idx].CurrentMaterial;
	}

}
