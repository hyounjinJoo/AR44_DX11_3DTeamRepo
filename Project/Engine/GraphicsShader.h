#pragma once
#include "IShader.h"
#include "define_GPU.h"

namespace mh
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

		virtual eResult Load(const std::filesystem::path& _path) override;

		eResult CreateByCompile(eGSStage _stage, const stdfs::path& _FullPath, const std::string_view _funcName);
		
		eResult CreateByHeader(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize);

		eResult CreateByCSO(eGSStage _stage, const stdfs::path& _FileName);

		eResult CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _VecLayoutDesc);

		void Binds();

		ID3D11InputLayout* GetInputLayout() { return mInputLayout.Get(); }
		ID3D11InputLayout** GetInputLayoutAddressOf() { return mInputLayout.GetAddressOf(); }

		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { mTopology = _topology; }
		void SetRSState(eRSType _state) { mRSType = _state; }
		void SetDSState(eDSType _state) { mDSType = _state; }
		void SetBSState(eBSType _state) { mBSType = _state; }

	private:
		eResult CreateShader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize);

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> mVecInputLayoutDesc;
		ComPtr<ID3D11InputLayout> mInputLayout;
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		eGSStage mStage;

		tShaderCode mArrShaderCode[(int)eGSStage::END];

		ComPtr<ID3D11VertexShader>		mVS;
		ComPtr<ID3D11HullShader>		mHS;
		ComPtr<ID3D11DomainShader>		mDS;
		ComPtr<ID3D11GeometryShader>	mGS;
		ComPtr<ID3D11PixelShader>		mPS;

		eRSType mRSType;
		eDSType mDSType;
		eBSType mBSType;

		ComPtr<ID3DBlob> mErrorBlob;
	};
}
