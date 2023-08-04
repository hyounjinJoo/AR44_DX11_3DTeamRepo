#include "PCH_Engine.h"

#include "GraphicsShader.h"
#include "GPUMgr.h"
#include "RenderMgr.h"

#include "PathMgr.h"
#include "json-cpp/jsonSaveLoad.h"

namespace mh
{
	namespace stdfs = std::filesystem;
	using namespace mh::define;

	GraphicsShader::GraphicsShader()
		: IShader(eResourceType::GraphicsShader)
		, mArrShaderCode{}
		, mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, mRSType(eRSType::SolidBack)
		, mDSType(eDSType::Less)
		, mBSType(eBSType::AlphaBlend)
		, mErrorBlob()
	{
	}

	GraphicsShader::~GraphicsShader()
	{

	}

	eResult GraphicsShader::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IShader::SaveJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}
		Json::Value& jVal = *_pJVal;

		//Input Layout Desc
		Json::MHSaveVector(_pJVal, JSON_KEY_PAIR(mVecInputLayoutDesc));

		//토폴로지
		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mTopology));

		//쉐이더 파일명
		{
			Json::Value& ShaderFileName = jVal[JSONKEY(mArrShaderCode)];
			for (int i = 0; i < (int)eGSStage::END; ++i)
			{
				ShaderFileName.append(mArrShaderCode[i].strKey);
			}
		}

		//래스터라이저 상태
		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mRSType));
		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mDSType));
		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mBSType));

		return eResult::Success;
	}

	eResult GraphicsShader::LoadJson(const Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IShader::LoadJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}
		const Json::Value& jVal = *_pJVal;

		//Input Layout Desc
		mVecInputLayoutDesc = Json::MHGetJsonVector(_pJVal, JSON_KEY_PAIR(mVecInputLayoutDesc));

		//토폴로지
		Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mTopology));

		//쉐이더
		{
			const std::vector<std::string>& vecStrKey = Json::MHGetJsonVectorPtr(_pJVal, JSONKEY(mArrShaderCode));
			for (size_t i = 0; i < vecStrKey.size(); ++i)
			{
				CreateByCSO((eGSStage)i, vecStrKey[i]);
				if ((size_t)eGSStage::END == i)
					break;
			}
		}

		//RS, DS, BS
		Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mRSType));
		Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mDSType));
		Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mBSType));

		return eResult::Success;
	}

	eResult GraphicsShader::Load(const std::filesystem::path& _path)
	{
		return eResult::Fail_NotImplemented;
	}

	eResult GraphicsShader::CreateByCompile(eGSStage _stage, const stdfs::path& _FullPath, const std::string_view _funcName)
	{
		mArrShaderCode[(int)_stage] = {};

		HRESULT hr = D3DCompileFromFile(
			_FullPath.wstring().c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			std::string(_funcName).c_str(),
			SHADER_VERSION::GS[(int)_stage],
			0u,
			0u,
			mArrShaderCode[(int)_stage].blob.ReleaseAndGetAddressOf(),
			mErrorBlob.ReleaseAndGetAddressOf()
		);

		if (FAILED(hr))
		{
			std::string ErrMsg = "Failed to compile Graphics GraphicsShader!\n\n";
			ErrMsg += "<Error Info>\n";
			ErrMsg += static_cast<const char*>(mErrorBlob->GetBufferPointer());
			ERROR_MESSAGE_A(ErrMsg.c_str());

			mErrorBlob = nullptr;

			return eResult::Fail_Compile;
		}

		return eResult::Success;
	}

	eResult GraphicsShader::CreateByHeader(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize)
	{
		mArrShaderCode[(int)_stage] = {};

		//헤더 형태로 만드는 쉐이더는 무조건 엔진 내부 기본 리소스라고 가정한다.
		SetEngineDefaultRes(true);

		tShaderCode& code = mArrShaderCode[(int)_stage];

		HRESULT hr = D3DCreateBlob(_ByteCodeSize, code.blob.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ERROR_MESSAGE_W(L"GraphicsShader를 저장할 Blob 생성에 실패했습니다.");
			
			return eResult::Fail_Create;
		}

		unsigned char* pCode = reinterpret_cast<unsigned char*>(code.blob->GetBufferPointer());
		//할당된 메모리 사이즈는 무조건 같아야 함
		size_t DestSize = code.blob->GetBufferSize();
		assert(_ByteCodeSize == DestSize);

		//데이터 복사
		memcpy_s(pCode, DestSize, _pByteCode, _ByteCodeSize);

		return CreateShader(_stage, pCode, DestSize);
	}

	eResult GraphicsShader::CreateByCSO(eGSStage _stage, const stdfs::path& _FileName)
	{
		//CSO 파일이 있는 폴더에 접근
		std::filesystem::path shaderBinPath = stdfs::current_path();
		shaderBinPath /= strKey::DirName_ShaderBin;
		shaderBinPath /= _FileName;

		if (false == stdfs::exists(shaderBinPath))
		{
			return eResult::Fail_PathNotExist;
		}

		//위에서 만든 파일명을 토대로 디스크에서 파일을 열어준다.
		std::ios_base::openmode openFlag = std::ios_base::ate | std::ios_base::binary; std::ios_base::in;
		std::ifstream sFile(shaderBinPath, openFlag);

		if (false == sFile.is_open())
		{
			std::wstring msg = L"Failed to open File.\n";
			msg += shaderBinPath.wstring();
			ERROR_MESSAGE_W(msg.c_str());
		}

		//파일이 열리면 지역변수 Blob을 만들어서 데이터를 옮긴다.
		tShaderCode sCode = {};
		sCode.strKey = _FileName.string();

		//Blob 내부에 공간을 할당.
		if (FAILED(D3DCreateBlob(sFile.tellg(), sCode.blob.GetAddressOf())))
		{
			ERROR_MESSAGE_W(L"쉐이더를 저장할 공간 할당에 실패했습니다.");
			return eResult::Fail_Create;
		}

		//커서를 처음으로 돌린 후 파일을 읽어준다.
		sFile.seekg(0, std::ios_base::beg);
		sFile.read((char*)sCode.blob->GetBufferPointer(), sCode.blob->GetBufferSize());
		sFile.close();

		//읽어온 바이트 코드로부터 쉐이더를 로딩해준다.
		//실패시 동적할당 해제하고 오류 발생
		eResult Result = CreateShader(_stage, sCode.blob->GetBufferPointer(), sCode.blob->GetBufferSize());
		if (eResultFail(Result))
		{
			ERROR_MESSAGE_W(L"쉐이더 생성 실패.");
			return Result;
		}

		return Result;
	}

	eResult GraphicsShader::CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _VecLayoutDesc)
	{
		ID3DBlob* VSBlobData = mArrShaderCode[(int)eGSStage::VS].blob.Get();

		if (nullptr == VSBlobData)
		{
			ERROR_MESSAGE_W(L"정점 쉐이더가 준비되지 않아서 Input Layout을 생성할 수 없습니다.");
			return eResult::Fail_Create;
		}

		mVecInputLayoutDesc = _VecLayoutDesc;

		if (FAILED(GPUMgr::Device()->CreateInputLayout(
			mVecInputLayoutDesc.data(),
			(uint)mVecInputLayoutDesc.size(),
			VSBlobData->GetBufferPointer(),
			VSBlobData->GetBufferSize(),
			mInputLayout.ReleaseAndGetAddressOf()
		)))
		{
			ERROR_MESSAGE_W(L"Input Layout 생성에 실패했습니다.");
			return eResult::Fail_Create;
		}


		return eResult::Success;
	}

	void GraphicsShader::Binds()
	{
		auto pContext = GPUMgr::Context();

		pContext->IASetPrimitiveTopology(mTopology);
		pContext->IASetInputLayout(mInputLayout.Get());
		pContext->VSSetShader(mVS.Get(), nullptr, 0);
		pContext->HSSetShader(mHS.Get(), nullptr, 0);
		pContext->DSSetShader(mDS.Get(), nullptr, 0);
		pContext->GSSetShader(mGS.Get(), nullptr, 0);
		pContext->PSSetShader(mPS.Get(), nullptr, 0);

		
		ID3D11RasterizerState*		rs = RenderMgr::GetRasterizerState(mRSType).Get();
		ID3D11DepthStencilState*	ds = RenderMgr::GetDepthStencilState(mDSType).Get();
		ID3D11BlendState*			bs = RenderMgr::GetBlendState(mBSType).Get();

		pContext->RSSetState(rs);
		pContext->OMSetDepthStencilState(ds, 0u);

		constexpr float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		pContext->OMSetBlendState(bs, blendFactor, UINT_MAX);
	}

	eResult GraphicsShader::CreateShader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize)
	{
		MH_ASSERT(_pByteCode && _ByteCodeSize);

		auto pDevice = GPUMgr::Device();
		switch (_stage)
		{
		case eGSStage::VS:
		{
			if (FAILED(pDevice->CreateVertexShader(_pByteCode, _ByteCodeSize, nullptr, mVS.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_W(L"Vertex GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}

		case eGSStage::HS:
		{
			if (FAILED(pDevice->CreateHullShader(_pByteCode, _ByteCodeSize, nullptr, mHS.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_W(L"Hull GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::DS:
		{
			if (FAILED(pDevice->CreateDomainShader(_pByteCode, _ByteCodeSize, nullptr, mDS.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_W(L"Domain GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::GS:
		{
			if (FAILED(pDevice->CreateGeometryShader(_pByteCode, _ByteCodeSize, nullptr, mGS.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_W(L"Geometry GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::PS:
		{
			if (FAILED(pDevice->CreatePixelShader(_pByteCode, _ByteCodeSize, nullptr, mPS.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_W(L"Pixel GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}

		}

		return eResult::Success;
	}

}
