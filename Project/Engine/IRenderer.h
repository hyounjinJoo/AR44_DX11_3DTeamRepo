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

		virtual void Render() = 0;

		void SetMesh(const std::shared_ptr<Mesh> _mesh);
		inline void SetMaterial(const std::shared_ptr <Material> _Mtrl, UINT _idx);
		inline std::shared_ptr<Mesh> GetMesh(UINT _containerIdx = 0u) const;
		inline const std::vector<std::shared_ptr<Material>>& const GetMaterials(UINT _containerIdx = 0u);
		UINT GetMaterialCount() { return (UINT)mMaterials.size(); }

		inline bool IsRenderReady() const;
	
	private:
		std::vector<tMeshContainer> mMeshContainers;
	};

	inline void IRenderer::SetMaterial(const std::shared_ptr<Material> _Mtrl, UINT _idx)
	{
		if ((UINT)mMaterials.size() <= _idx)
			mMaterials.resize(_idx + 1u);

		mMaterials[_idx].SharedMaterial = _Mtrl;
		mMaterials[_idx].CurrentMaterial = _Mtrl;
	}

	inline std::shared_ptr<Mesh> IRenderer::GetMesh(UINT _containerIdx) const
	{
		std::shared_ptr<Mesh> pMesh = nullptr;

		if (_containerIdx < (UINT)mMeshContainers.size())
			pMesh = mMeshContainers[_containerIdx].pMesh;

		return pMesh;
	}

	inline const std::vector<std::shared_ptr<Material>>& const GetMaterials(UINT _containerIdx)
	{
		if (nullptr == mMeshContainers[_containerIdx].)
		{
			mMaterials[_idx].CurrentMaterial = mMaterials[_idx].SharedMaterial;
		}

		return mMaterials[_idx].CurrentMaterial;
	}

	inline bool IRenderer::IsRenderReady() const
	{
		bool bReady = false;

		//meshContainers가 하나라도 존재 + 첫번째 메쉬의 mesh가 존재 + material도 존재 할 경우에만
		if (
			false == mMeshContainers.empty() &&
			mMeshContainers[0].pMesh &&
			false == mMeshContainers[0].pMaterials.empty(
			))
			bReady = true;


		return bReady;
	}

}
