#pragma once
#include "IRenderer.h"

namespace mh
{
	class Com_Renderer_Mesh : public IRenderer
	{
	public:
		Com_Renderer_Mesh();

		Com_Renderer_Mesh(const Com_Renderer_Mesh& _other);
		CLONE(Com_Renderer_Mesh);

		virtual ~Com_Renderer_Mesh();

		//virtual void Init() override;
		//virtual void Update() override;
		virtual void FixedUpdate() override {};
		virtual void Render() override;

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		void SetMesh(const std::shared_ptr<Mesh> _mesh);

		//인스턴싱 구현 전까지는 일단 Material을 복사해서 사용
		inline void SetMaterial(const std::shared_ptr <Material> _Mtrl, UINT _idx);
		inline Material* SetMaterialMode(UINT _idx, eMaterialMode _mode);

		std::shared_ptr<Mesh> GetMesh() { return mMesh; }

		inline std::shared_ptr<Material> GetSharedMaterial(UINT _idx);
		inline Material* GetDynamicMaterial(UINT _idx);
		inline Material* GetCurrentMaterial(UINT _idx);

		UINT GetMaterialCount() { return (UINT)mMaterials.size(); }
		bool IsRenderReady() const { return (mMesh && false == mMaterials.empty()); }

		virtual define::eRenderingMode GetRenderingMode() override;

	private:
		inline tMaterialSet* GetMaterialSet(UINT _idx);
	private:
		std::shared_ptr<Mesh> mMesh;
		std::vector<tMaterialSet> mMaterials;
	};

	inline void Com_Renderer_Mesh::SetMaterial(const std::shared_ptr<Material> _Mtrl, UINT _idx)
	{
		if ((UINT)mMaterials.size() <= _idx)
			mMaterials.resize(_idx + 1u);

		mMaterials[_idx].SharedMaterial = _Mtrl;
		mMaterials[_idx].CurrentMaterial = mMaterials[_idx].SharedMaterial.get();
	}

	inline Material* Com_Renderer_Mesh::SetMaterialMode(UINT _idx, eMaterialMode _mode)
	{
		tMaterialSet* mtrlSet = GetMaterialSet(_idx);
		Material* mtrl = nullptr;
		if (mtrlSet)
		{
			if (eMaterialMode::Shared == _mode)
			{
				mtrlSet->CurrentMaterial = mtrlSet->SharedMaterial.get();
				mtrlSet->MaterialMode = _mode;
			}
			else if (eMaterialMode::Dynamic == _mode)
			{
				//Dynamic Material이 없는데 Shared Material이 있을 경우 복사해서 새로 생성
				if (mtrlSet->SharedMaterial)
				{
					if (nullptr == mtrlSet->DynamicMaterial)
					{
						mtrlSet->DynamicMaterial = std::unique_ptr<Material>(mtrlSet->SharedMaterial->Clone());
					}
					mtrlSet->CurrentMaterial = mtrlSet->DynamicMaterial.get();
					mtrlSet->MaterialMode = _mode;
				}
			}
			mtrl = mtrlSet->CurrentMaterial;
		}

		return mtrl;
	}


	inline Material* Com_Renderer_Mesh::GetCurrentMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			retMtrl = mMaterials[_idx].CurrentMaterial;
		}

		return retMtrl;
	}

	inline std::shared_ptr<Material> Com_Renderer_Mesh::GetSharedMaterial(UINT _idx)
	{
		std::shared_ptr<Material> retMtrl = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			retMtrl = mMaterials[_idx].SharedMaterial;
		}
		return retMtrl;
	}

	inline Material* Com_Renderer_Mesh::GetDynamicMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			retMtrl = mMaterials[_idx].DynamicMaterial.get();
		}
		return retMtrl;
	}

	inline define::eRenderingMode Com_Renderer_Mesh::GetRenderingMode()
	{
		eRenderingMode mode = eRenderingMode::None;

		Material* mtrl = GetCurrentMaterial(0u);
		if (mtrl)
		{
			mode = mtrl->GetRenderingMode();
		}

		return mode;
	}

	inline tMaterialSet* Com_Renderer_Mesh::GetMaterialSet(UINT _idx)
	{
		tMaterialSet* mtrlSet = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			mtrlSet = &mMaterials[_idx];
		}
		return mtrlSet;
	}

}
