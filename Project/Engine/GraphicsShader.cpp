#include "PCH_Engine.h"

#include "GraphicsShader.h"
#include "GPUMgr.h"
#include "RenderMgr.h"

#include "PathMgr.h"
#include "json-cpp/jsonSaveLoad.h"

namespace mh
{
	std::unordered_set<std::string> GraphicsShader::mSemanticNames{};

	struct D3D11InputElementDescWithoutName
	{
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
	};



	
	using namespace mh::define;

	GraphicsShader::GraphicsShader()
		: IShader(eResourceType::GraphicsShader)
		, mArrShaderCode{}
		, mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, mRSType(eRSType::SolidBack)
		, mDSType(eDSType::Less)
		, mBSType(eBSType::Default)
		, mErrorBlob()
		, mbEditMode(false)
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

		Json::Value& jsonInputLayouts = jVal[JSON_KEY(mInputLayoutDescs)];
		//Input Layout Desc
		for (size_t i = 0; i < mInputLayoutDescs.size(); ++i)
		{
			Json::Value InputElement{};

			std::string strSemanticName;
			if (mInputLayoutDescs[i].SemanticName)
			{
				strSemanticName = mInputLayoutDescs[i].SemanticName;
			}
			InputElement.append(strSemanticName);

			D3D11InputElementDescWithoutName desc{};
			desc.SemanticIndex = mInputLayoutDescs[i].SemanticIndex;
			desc.Format = mInputLayoutDescs[i].Format;
			desc.InputSlot = mInputLayoutDescs[i].InputSlot;
			desc.AlignedByteOffset = mInputLayoutDescs[i].AlignedByteOffset;
			desc.InputSlotClass = mInputLayoutDescs[i].InputSlotClass;
			desc.InstanceDataStepRate = mInputLayoutDescs[i].InstanceDataStepRate;

			std::string converted = Json::MH::ConvertWrite(desc);

			InputElement.append(converted);

			jsonInputLayouts.append(InputElement);
		}

		//Json::MH::SaveValueVector(_pJVal, JSON_KEY_PAIR(mInputLayoutDescs));

		//토폴로지
		Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mTopology));

		//쉐이더 파일명
		{
			Json::Value& ShaderFileName = jVal[JSON_KEY(mArrShaderCode)];
			for (int i = 0; i < (int)eGSStage::END; ++i)
			{
				ShaderFileName.append(mArrShaderCode[i].strKey);
			}
		}

		//래스터라이저 상태
		Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mRSType));
		Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mDSType));
		Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mBSType));

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
		mInputLayoutDescs.clear();
		if (jVal.isMember(JSON_KEY(mInputLayoutDescs)))
		{
			const Json::Value& jsonInputLayouts = jVal[JSON_KEY(mInputLayoutDescs)];

			if (jsonInputLayouts.isArray())
			{
				for (
					Json::ValueConstIterator iter = jsonInputLayouts.begin();
					iter != jsonInputLayouts.end();
					++iter
					)
				{
					if (2 > iter->size())
					{
						NOTIFICATION_W(L"Input Layout Desc 정보가 누락되어 있습니다.");
						return eResult::Fail_InValid;
					}

					const auto& pair = mSemanticNames.insert((*iter)[0].asString());

					D3D11InputElementDescWithoutName descValue = Json::MH::ConvertRead<D3D11InputElementDescWithoutName>((*iter)[1]);

					D3D11_INPUT_ELEMENT_DESC desc{};
					desc.SemanticName = pair.first->c_str();
					desc.SemanticIndex = descValue.SemanticIndex;
					desc.Format = descValue.Format;
					desc.InputSlot = descValue.InputSlot;
					desc.AlignedByteOffset = descValue.AlignedByteOffset;
					desc.InputSlotClass = descValue.InputSlotClass;
					desc.InstanceDataStepRate = descValue.InstanceDataStepRate;

					mInputLayoutDescs.push_back(desc);
				}
			}
		}

		//토폴로지
		Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mTopology));

		//쉐이더
		{
			const std::vector<std::string>& vecStrKey = Json::MH::LoadPtrStrKeyVector(_pJVal, JSON_KEY_PAIR(mArrShaderCode));

			//에딧 모드가 아닐 경우에만 로드
			for (size_t i = 0; i < mArrShaderCode.size(); ++i)
			{
				if (false == mbEditMode && false == vecStrKey[i].empty())
				{
					CreateByCSO((eGSStage)i, vecStrKey[i]);
					if ((size_t)eGSStage::END == i)
						break;
				}
				else
				{
					mArrShaderCode[i].strKey = vecStrKey[i];
				}
			}

		}

		//RS, DS, BS
		Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mRSType));
		Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mDSType));
		Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mBSType));

		if (false == mbEditMode)
		{
			result = CreateInputLayout();
			if (eResultFail(result))
			{
				return result;
			}
		}


		return eResult::Success;
	}

	eResult GraphicsShader::Save(const std::filesystem::path& _path)
	{
		std::fs::path FilePath = PathMgr::GetContentPathRelative(eResourceType::GraphicsShader);
		FilePath /= _path;
		{
			std::fs::path fileDir = FilePath.parent_path();
			if (false == std::fs::exists(fileDir))
			{
				std::fs::create_directories(fileDir);
			}
		}
		FilePath.replace_extension(define::strKey::Ext_ShaderSetting);

		//파일 열고
		std::ofstream ofs(FilePath);
		if (false == ofs.is_open())
			return eResult::Fail_OpenFile;

		//json 저장하고
		Json::Value jVal;
		eResult result = SaveJson(&jVal);
		if (eResultFail(result))
			return result;

		//파일로 저장
		ofs << jVal;

		//닫아주기
		ofs.close();

		return eResult::Success;
	}

	eResult GraphicsShader::Load(const std::filesystem::path& _path)
	{
		std::fs::path FilePath = PathMgr::GetContentPathRelative(eResourceType::GraphicsShader);
		FilePath /= _path;
		FilePath.replace_extension(define::strKey::Ext_ShaderSetting);

		std::ifstream ifs(FilePath);
		if (false == ifs.is_open())
			return eResult::Fail_OpenFile;

		Json::Value jVal{};
		ifs >> jVal;
		ifs.close();

		eResult result = LoadJson(&jVal);

		return result;
	}

	eResult GraphicsShader::CreateByCompile(eGSStage _stage, const std::fs::path& _FullPath, const std::string_view _funcName)
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

	eResult GraphicsShader::CreateByCSO(eGSStage _stage, const std::fs::path& _FileName)
	{
		//CSO 파일이 있는 폴더에 접근
		std::filesystem::path shaderBinPath = PathMgr::GetShaderCSOPath();
		shaderBinPath /= _FileName;

		if (false == std::fs::exists(shaderBinPath))
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

		mArrShaderCode[(int)_stage] = sCode;

		return Result;
	}

	eResult GraphicsShader::CreateInputLayout()
	{
		ID3DBlob* VSBlobData = mArrShaderCode[(int)eGSStage::VS].blob.Get();

		if (nullptr == VSBlobData)
		{
			ERROR_MESSAGE_W(L"정점 쉐이더가 준비되지 않아서 Input Layout을 생성할 수 없습니다.");
			return eResult::Fail_Create;
		}
		else if (mInputLayoutDescs.empty())
		{
			ERROR_MESSAGE_W(L"입력 레이아웃이 설정되어있지 않아 Input Layout을 생성할 수 없습니다.");
			return eResult::Fail_Create;
		}

		if (FAILED(GPUMgr::Device()->CreateInputLayout(
			mInputLayoutDescs.data(),
			(uint)mInputLayoutDescs.size(),
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

	void GraphicsShader::BindData()
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
		pContext->OMSetDepthStencilState(ds, 10u);

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
