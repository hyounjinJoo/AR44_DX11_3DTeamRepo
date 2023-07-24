#include "EnginePCH.h"

#include "ComputeShader.h"
#include "GraphicDevice_DX11.h"

namespace mh::graphics
{
	ComputeShader::ComputeShader(UINT _threadGroupX, UINT _threadGroupY, UINT _threadGroupZ)
		: GameResource(enums::eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mThreadGroupCountX(_threadGroupX)
		, mThreadGroupCountY(_threadGroupY)
		, mThreadGroupCountZ(_threadGroupZ)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)
	{
	}
	ComputeShader::ComputeShader()
		: GameResource(enums::eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mThreadGroupCountX(0)
		, mThreadGroupCountY(0)
		, mThreadGroupCountZ(0)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)
	{
		mThreadGroupCountX = 32;
		mThreadGroupCountY = 32;
		mThreadGroupCountZ = 1;
	}
	ComputeShader::~ComputeShader()
	{
	}
	HRESULT ComputeShader::Load(const std::filesystem::path& _path)
	{
		return E_NOTIMPL;
	}
	bool ComputeShader::Create(const std::wstring& _file, const std::string& _funcName)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob = nullptr;

		// Vertex Shader
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\SHADER_SOURCE\\";

		std::wstring shaderPath(path.c_str());
		shaderPath += _file;

		D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _funcName.c_str(), "cs_5_0", 0, 0
			, mCSBlob.GetAddressOf()
			, mErrorBlob.GetAddressOf());

		//if (mErrorBlob)
		//{
		//	OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
		//	mErrorBlob->Release();
		//}

		GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer()
			, mCSBlob->GetBufferSize()
			, nullptr
			, mCS.GetAddressOf());

		return true;
	}
	void ComputeShader::OnExcute()
	{
		Binds();

		GetDevice()->BindComputeShader(mCS.Get(), nullptr, 0);
		GetDevice()->Dispatch(mGroupX, mGroupY, mGroupZ);

		Clear();
	}

	void ComputeShader::Binds()
	{
	}
	void ComputeShader::Clear()
	{
	}
}
