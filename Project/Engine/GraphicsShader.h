#pragma once
#include "IShader.h"
#include "define_GPU.h"

namespace mh::GPU
{
	using Microsoft::WRL::ComPtr;
	namespace stdfs = std::filesystem;

	struct tShaderCode
	{
		ComPtr<ID3DBlob> blob;
		std::string strKey;
	};

	class GraphicsShader : public IShader
	{
	public:
		GraphicsShader();
		virtual ~GraphicsShader();

		virtual HRESULT Load(const std::filesystem::path& _path) override;

		eResult CreateByCompile(GPU::eGSStage _stage, const stdfs::path& _FullPath, const std::string_view _funcName);
		
		eResult CreateByHeader(GPU::eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize);

		eResult CreateByCSO(GPU::eGSStage _stage, const stdfs::path& _FileName);

		eResult CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _VecLayoutDesc);

		void Binds();

		ID3D11InputLayout* GetInputLayout() { return mInputLayout.Get(); }
		ID3D11InputLayout** GetInputLayoutAddressOf() { return mInputLayout.GetAddressOf(); }

		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { mTopology = _topology; }
		void SetRSState(GPU::eRSType _state) { mRSType = _state; }
		void SetDSState(GPU::eDSType _state) { mDSType = _state; }
		void SetBSState(GPU::eBSType _state) { mBSType = _state; }

	private:
		eResult CreateShader(GPU::eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize);

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> mVecInputLayoutDesc;
		ComPtr<ID3D11InputLayout> mInputLayout;
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		GPU::eGSStage mStage;

		tShaderCode mArrShaderCode[(int)GPU::eGSStage::END];

		ComPtr<ID3D11VertexShader>		mVS;
		ComPtr<ID3D11HullShader>		mHS;
		ComPtr<ID3D11DomainShader>		mDS;
		ComPtr<ID3D11GeometryShader>	mGS;
		ComPtr<ID3D11PixelShader>		mPS;

		GPU::eRSType mRSType;
		GPU::eDSType mDSType;
		GPU::eBSType mBSType;

		ComPtr<ID3DBlob> mErrorBlob;
	};
}
