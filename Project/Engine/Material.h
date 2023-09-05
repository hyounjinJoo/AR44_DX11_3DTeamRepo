#pragma once
#include "define_Enum.h"

#include "IRes.h"
#include "GraphicsShader.h"
#include "Texture.h"
#include "RenderMgr.h"

//에디터 전방선언
namespace gui
{
	class guiMaterialEditor;
}

namespace mh
{
	class Material : public IRes
	{
		friend class gui::guiMaterialEditor;
	public:
		Material();

		Material(const Material& _other);
		CLONE(Material);

		virtual ~Material();

		virtual eResult Save(const std::fs::path& _filePath) override;
		virtual eResult Load(const std::fs::path& _filePath) override;

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		void BindData();
		void UnBindData();

		void SetShader(std::shared_ptr<GraphicsShader> _shader) { mShader = _shader; }
		std::shared_ptr<GraphicsShader> GetShader() const { return mShader; }


		inline void SetTexture(eTextureSlot slot, std::shared_ptr<Texture> _texture);
		std::shared_ptr<Texture> GetTexture(eTextureSlot _slot) const { return mTextures[(uint)_slot]; }

		eRenderingMode GetRenderingMode() const { return mMode; }
		void SetRenderingMode(eRenderingMode _mode) { mMode = _mode; }

		inline void SetMaterialCoefficient(const float4& _vDiff, const float4& _vSpec, const float4& _vAmb, const float4& _vEmis);

		void SetAnim3D(bool _bAnim3D) { mCB.bAnim = (BOOL)_bAnim3D; }
		void SetBoneCount(int _iBoneCount) { mCB.BoneCount = _iBoneCount; }

			
	private:
		eRenderingMode mMode;
		tCB_MaterialData mCB;
		
		std::shared_ptr<GraphicsShader> mShader;
		std::array<std::shared_ptr<Texture>, (int)eTextureSlot::END> mTextures;
	};

	inline void Material::SetMaterialCoefficient(const float4& _vDiff, const float4& _vSpec, const float4& _vAmb, const float4& _vEmis)
	{
		mCB.Diff = _vDiff;
		mCB.Spec = _vSpec;
		mCB.Amb = _vAmb;
		mCB.Emv = _vEmis;
	}

	inline void Material::SetTexture(eTextureSlot slot, std::shared_ptr<Texture> _texture)
	{
		mTextures[(UINT)slot] = _texture;
		BOOL bTex = nullptr != _texture ? TRUE : FALSE;
		switch ((UINT)slot)
		{
		case 0u:
			mCB.bTex_0 = bTex;
			break;
		case 1u:
			mCB.bTex_1 = bTex;
			break;
		case 2u:
			mCB.bTex_2 = bTex;
			break;
		case 3u:
			mCB.bTex_3 = bTex;
			break;
		case 4u:
			mCB.bTex_4 = bTex;
			break;
		case 5u:
			mCB.bTex_5 = bTex;
			break;
		case 6u:
			mCB.bTex_6 = bTex;
			break;
		case 7u:
			mCB.bTex_7 = bTex;
			break;
		//case 8u:
		//	mCB.bTexCube_0 = bTex;
		//	break;
		default:
			MH_ASSERT(false);
			break;
		}
	}
}

