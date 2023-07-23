#include "EnginePCH.h"

#include "Shader.h"
#include "GraphicDevice_DX11.h"
#include "Renderer.h"

using namespace mh::graphics;

namespace mh
{
	Shader::Shader()
		: GameResource(eResourceType::GraphicShader)
		, mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, mRSType(eRSType::SolidBack)
		, mDSType(eDSType::Less)
		, mBSType(eBSType::AlphaBlend)
		, mStage()
	{
	}

	Shader::~Shader()
	{

	}

	HRESULT Shader::Load(const std::wstring& _path)
	{
		return E_NOTIMPL;
	}

	void Shader::Create(graphics::eShaderStage _stage, const std::wstring& _file, const std::string& _funcName)
	{
		mErrorBlob = nullptr;

		// Vertex Shader
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\SHADER_SOURCE\\";

		std::wstring shaderPath(path.c_str());
		shaderPath += _file;
		
		if (_stage == graphics::eShaderStage::VS)
		{
			D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
								, _funcName.c_str() , "vs_5_0", 0, 0
								, mVSBlob.GetAddressOf()
								, mErrorBlob.GetAddressOf());

			//if (mErrorBlob)
			//{
			//	OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
			//	mErrorBlob->Release();
			//}

			GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer()
														, mVSBlob->GetBufferSize()
														, nullptr
														, mVS.GetAddressOf());
		}
		else if (_stage == graphics::eShaderStage::PS)
		{
			D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
				, _funcName.c_str(), "ps_5_0", 0, 0
				, mPSBlob.GetAddressOf()
				, mErrorBlob.GetAddressOf());

			/*if (mErrorBlob)
			{
				OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
				mErrorBlob->Release();
			}*/

			GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer()
				, mPSBlob->GetBufferSize()
				, nullptr
				, mPS.GetAddressOf());
		}
		else if (_stage == graphics::eShaderStage::GS)
		{
			D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
				, _funcName.c_str(), "gs_5_0", 0, 0
				, mGSBlob.GetAddressOf()
				, mErrorBlob.GetAddressOf());

			//if (mErrorBlob)
			//{
			//	OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
			//	mErrorBlob->Release();
			//}

			GetDevice()->CreateGeometryShader(mGSBlob->GetBufferPointer()
				, mGSBlob->GetBufferSize()
				, nullptr
				, mGS.GetAddressOf());
		}
	}

	void Shader::Binds()
	{
		GetDevice()->BindPrimitiveTopology(mTopology);
		GetDevice()->BindInputLayout(mInputLayout.Get());

		GetDevice()->BindVertexShader(mVS.Get(), nullptr, 0);
		GetDevice()->BindHullShader(mHS.Get(), nullptr, 0);
		GetDevice()->BindDomainShader(mDS.Get(), nullptr, 0);
		GetDevice()->BindGeometryShader(mGS.Get(), nullptr, 0);
		GetDevice()->BindPixelShader(mPS.Get(), nullptr, 0);

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs = renderer::gRasterizerStates[(UINT)mRSType];
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ds = renderer::gDepthStencilStates[(UINT)mDSType];
		Microsoft::WRL::ComPtr<ID3D11BlendState> bs = renderer::gBlendStates[(UINT)mBSType];

		GetDevice()->BindRasterizerState(rs.Get());
		GetDevice()->BindDepthStencilState(ds.Get());
		GetDevice()->BindBlendState(bs.Get());
	}

}
