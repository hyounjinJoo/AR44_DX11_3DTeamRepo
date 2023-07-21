#pragma once
#include "GameResource.h"
#include "Graphics.h"

namespace mh
{
	class Shader : public GameResource
	{
	public:
		Shader();
		virtual ~Shader();

		virtual HRESULT Load(const std::wstring& _path) override;

		void Create(graphics::eShaderStage _stage, const std::wstring& _file, const std::string& _funName);
		void Binds();

		ID3D11InputLayout* GetInputLayout() { return mInputLayout.Get(); }
		ID3D11InputLayout** GetInputLayoutAddressOf() { return mInputLayout.GetAddressOf(); }

		void* GetVSBlobBufferPointer() { return mVSBlob->GetBufferPointer(); }
		SIZE_T GetVSBlobBufferSize() { return mVSBlob->GetBufferSize(); }

		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { mTopology = _topology; }
		void SetRSState(graphics::eRSType _state) { mRSType = _state; }
		void SetDSState(graphics::eDSType _state) { mDSType = _state; }
		void SetBSState(graphics::eBSType _state) { mBSType = _state; }

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		graphics::eShaderStage mStage;

		Microsoft::WRL::ComPtr<ID3DBlob> mVSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mHSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mDSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mGSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mPSBlob;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVS;
		Microsoft::WRL::ComPtr<ID3D11HullShader> mHS;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> mDS;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPS;

		graphics::eRSType mRSType;
		graphics::eDSType mDSType;
		graphics::eBSType mBSType;

		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;
	};
}
