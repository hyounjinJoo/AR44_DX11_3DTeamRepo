#include "PCH_Engine.h"

#include "PathMgr.h"

#include "ComputeShader.h"
#include "GPUMgr.h"
#include "RenderMgr.h"
#include "ConstBuffer.h"

#include "define_GPU.h"
#include "define_Res.h"



namespace mh
{
	

	ComputeShader::ComputeShader(uint3 _threadsPerGroup)
		: IShader(define::eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mCB_ComputeShader{ _threadsPerGroup,  }
	{
		//스레드가 하나라도 0이 들어있으면 안됨
		MH_ASSERT(_threadsPerGroup.x && _threadsPerGroup.y && _threadsPerGroup.z);

	}

	ComputeShader::~ComputeShader()
	{
	}

	eResult ComputeShader::Load(const std::filesystem::path& _path)
	{
		std::fs::path FilePath = PathMgr::GetShaderCSOPath();
		FilePath /= _path;
		FilePath.replace_extension(define::strKey::Ext_CompiledShader);

		return CreateByCSO(FilePath);
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
		if (false == BindData())
			return;

		//데이터 카운트가 하나라도 0일경우 계산 불가
		 if (false == (
			 mCB_ComputeShader.TotalDataCount.x && 
			 mCB_ComputeShader.TotalDataCount.y && 
			 mCB_ComputeShader.TotalDataCount.z))
		 {
			 UnBindData();
			 return;
		 }
			
		
		//상수버퍼를 통해 데이터 수를 업로드
		static ConstBuffer* const pCB = RenderMgr::GetConstBuffer(define::eCBType::ComputeShader);
		pCB->SetData(&mCB_ComputeShader);
		pCB->BindData();

		//쉐이더 바인딩
		GPUMgr::Context()->CSSetShader(mCS.Get(), nullptr, 0);
		GPUMgr::Context()->Dispatch(mCB_ComputeShader.NumGroup.x, mCB_ComputeShader.NumGroup.y, mCB_ComputeShader.NumGroup.z);

		//데이터 정리
		UnBindData();
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
