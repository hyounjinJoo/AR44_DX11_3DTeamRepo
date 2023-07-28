#pragma once
#include "IRes.h"
#include "GraphicsShader.h"
#include "Texture.h"
#include "RenderMgr.h"

namespace mh::GPU
{
	class Material : public IRes
	{
	public:
		Material();
		virtual ~Material();

		virtual HRESULT Load(const std::filesystem::path& _path) override;

		void SetData(eGPUParam _param, void* _data);
		void Bind();
		void Clear();

		void SetShader(std::shared_ptr<GraphicsShader> _shader) { mShader = _shader; }
		void SetTexture(eTextureSlot slot, std::shared_ptr<Texture> _texture) { mTexture[(UINT)slot] = _texture; }

		std::shared_ptr<GraphicsShader> GetShader() const { return mShader; }
		std::shared_ptr<Texture> GetTexture(eTextureSlot _slot) const { return mTexture[(UINT)_slot]; }

		eRenderingMode GetRenderingMode() const { return mMode; }
		void SetRenderingMode(eRenderingMode _mode) { mMode = _mode; }

	private:
		std::shared_ptr<GraphicsShader> mShader;
		std::shared_ptr<Texture> mTexture[(UINT)eTextureSlot::End];

		
		MaterialCB mCB;
		eRenderingMode mMode;
	};
}

