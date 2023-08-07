#include "PCH_Engine.h"

#include "PathMgr.h"

#include "ComputeShader.h"
#include "GPUMgr.h"

#include "define_GPU.h"
#include "define_Res.h"

namespace mh
{
	namespace stdfs = std::filesystem;

	ComputeShader::ComputeShader(uint _threadGroupX, uint _threadGroupY, uint _threadGroupZ)
		: IShader(define::eResourceType::ComputeShader)
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
		: IShader(define::eResourceType::ComputeShader)
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
	eResult ComputeShader::Load(const std::filesystem::path& _path)
	{
		return eResult::Fail_NotImplemented;
	}
	eResult ComputeShader::CreateByCompile(const std::filesystem::path& _FullPath, const std::string_view _funcName)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob = nullptr;

		if (FAILED(D3DCompileFromFile(
			_FullPath.wstring().c_str(), 
			nullptr, 
			D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			std::string(_funcName).c_str(), 
			mh::SHADER_VERSION::CS, 
			0, 
			0, 
			mCSBlob.GetAddressOf(), 
			mErrorBlob.GetAddressOf())))
		{
			std::string ErrMsg = "Failed to compile Compute GraphicsShader!\n\n";
			ErrMsg += "<Error Info>\n";
			ErrMsg += static_cast<const char*>(mErrorBlob->GetBufferPointer());
			ERROR_MESSAGE_A(ErrMsg.c_str());

			mErrorBlob = nullptr;

			return eResult::Fail_Compile;
		}


		return eResult::Success;
	}

	eResult ComputeShader::CreateByHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize)
	{
		//헤더 형태로 만드는 쉐이더는 무조건 엔진 내부 기본 리소스라고 가정한다.
		SetEngineDefaultRes(true);

		HRESULT hr = D3DCreateBlob(_ByteCodeSize, mCSBlob.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ERROR_MESSAGE_W(L"GraphicsShader를 저장할 Blob 생성에 실패했습니다.");

			return eResult::Fail_Create;
		}

		unsigned char* pCode = reinterpret_cast<unsigned char*>(mCSBlob->GetBufferPointer());
		//할당된 메모리 사이즈는 무조건 같아야 함
		size_t DestSize = mCSBlob->GetBufferSize();
		MH_ASSERT(_ByteCodeSize == DestSize);

		//데이터 복사
		memcpy_s(pCode, DestSize, _pByteCode, _ByteCodeSize);

		return CreateShader(pCode, DestSize);
	}

	eResult ComputeShader::CreateByCSO(const std::filesystem::path& _FileName)
	{
		return eResult::Fail_NotImplemented;
	}

	void ComputeShader::OnExcute()
	{
		Binds();

		auto pContext = GPUMgr::Context();
		pContext->CSSetShader(mCS.Get(), nullptr, 0);
		pContext->Dispatch(mGroupX, mGroupY, mGroupZ);

		Clear();
	}

	void ComputeShader::Binds()
	{
	}
	void ComputeShader::Clear()
	{
	}
	eResult ComputeShader::CreateShader(const void* _pByteCode, size_t _ByteCodeSize)
	{
		eResult result = eResult::Fail_Create;

		if (SUCCEEDED(GPUMgr::Device()->CreateComputeShader(
			_pByteCode,
			_ByteCodeSize,
			nullptr,
			mCS.ReleaseAndGetAddressOf()))
			)
		{
			result = eResult::Success;
		}

		return result;
	}
}
