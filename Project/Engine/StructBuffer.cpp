#include "EnginePCH.h"
#include "StructBuffer.h"

#include "GPUMgr.h"
#include "RenderMgr.h"
#include "ConstBuffer.h"

namespace mh
{
	StructBuffer::StructBuffer()
		: mBufferDesc()
		, mSBufferDesc()
		, mbSBufferDescSet()
		, mElementStride()
		, mElementCount()
		, mElementCapacity()
		, mStructBuffer()
		, mSRV()
		, mUAV()
		, mStagingBuffer()
		, mCurBoundView()
	{
	}

	StructBuffer::StructBuffer(const tSBufferDesc& _tDesc)
		: mBufferDesc()
		, mSBufferDesc()
		, mbSBufferDescSet()
		, mElementStride()
		, mElementCount()
		, mElementCapacity()
		, mStructBuffer()
		, mSRV()
		, mUAV()
		, mStagingBuffer()
		, mCurBoundView()

	{
		SetDesc(_tDesc);
	}


	//StructBuffer::StructBuffer(const StructBuffer& _other)
	//	: IEntity(_other)
	//	, mBufferDesc(_other.mBufferDesc)
	//	, mSBufferDesc(_other.mSBufferDesc)
	//	, mbSBufferDescSet(_other.mbSBufferDescSet)
	//	, mElementStride()
	//	, mElementCount()
	//	, mElementCapacity()
	//	, mStructBuffer()
	//	, mSRV()
	//	, mUAV()
	//	, mStagingBuffer()
	//	, mCurBoundView()
	//{
	//	SetDesc(_other.mSBufferDesc);

	//	if (mbSBufferDescSet)
	//	{
	//		Create(_other.mElementStride, _other.mElementCapacity, nullptr, 0u);
	//	}
	//}

	StructBuffer::~StructBuffer()
	{
		UnBind();
	}




	HRESULT StructBuffer::Create(size_t _uElemStride, size_t _uElemCapacity, void* _pInitialData, size_t _uElemCount)
	{
		if (false == mbSBufferDescSet)
		{
			ERROR_MESSAGE_A("Must Set SBuffer Description!");
			return E_FAIL;
		}

		//상수버퍼와 마찬가지로 16바이트 단위로 정렬되어 있어야 함.s
		else if (0 != _uElemStride % 16)
		{
			ERROR_MESSAGE_A("The byte size of the structured buffer must be a multiple of 16.");
			return E_FAIL;
		}

		else if (_uElemCapacity < _uElemCount)
		{
			ERROR_MESSAGE_A("Element capacity of structured buffer must be more than element of input data!");
			return E_FAIL;
		}

		//재할당 하기 전 바인딩된 리소스가 있다면 unbind
		UnBind();

		//상수버퍼와는 다르게 버퍼 재할당이 가능함. 먼저 기존 버퍼의 할당을 해제한다.(ComPtr을 통해 관리가 이루어지므로 nullptr로 바꿔주면 됨.)
		mElementStride = (UINT)_uElemStride;
		mElementCount = (UINT)_uElemCount;

		mElementCapacity = (UINT)_uElemCapacity;

		mBufferDesc.StructureByteStride = mElementStride;
		mBufferDesc.ByteWidth = mElementStride * mElementCapacity;

		switch (mSBufferDesc.eSBufferType)
		{
			//일반적인 GPU에서 읽기만 가능한 구조화 버퍼
		case eStructBufferType::READ_ONLY:
		{
			//초깃값 업로드 및 바인딩
			D3D11_SUBRESOURCE_DATA* pData = nullptr;
			D3D11_SUBRESOURCE_DATA Data = {};
			if (nullptr != _pInitialData)
			{
				Data.pSysMem = _pInitialData;

				//이 값은 무시됨. BufferDesc.ByteWidth 값만 영향을 미치는 것을 확인함.
				Data.SysMemPitch = mElementStride * (UINT)_uElemCount;
				Data.SysMemSlicePitch = mBufferDesc.StructureByteStride;
				pData = &Data;
				mCurBoundView = eBufferViewType::SRV;
			}


			//구조화버퍼 생성. ReleaseAndGetAddressOf() 함수를 통해서 기존 구조화 버퍼가 있다면 날려버리고 생성
			if (FAILED(GPUMgr::GetInst()->GetDevice()->CreateBuffer(&mBufferDesc, pData, mStructBuffer.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_A("Failed to create Structured Buffer!");
				return E_FAIL;
			}

			//Read Only의 경우 바로 SRV 생성
			CreateSRV();

			break;
		}

		//Compute Shader과 결합해서 사용하는, GPU에서 조작 가능한 SBuffer
		//이 경우에는 CPU와 데이터 전송이 불가능하므로 데이터 전송용 Staging Buffer를 중간 단계로 사용해야 한다.
		case eStructBufferType::READ_WRITE:
		{
			//초깃값 지정
			D3D11_SUBRESOURCE_DATA* pData = nullptr;
			D3D11_SUBRESOURCE_DATA Data = {};
			if (nullptr != _pInitialData)
			{
				Data.pSysMem = _pInitialData;
				Data.SysMemPitch = mElementStride * (UINT)_uElemCount;
				Data.SysMemSlicePitch = mBufferDesc.StructureByteStride;
				pData = &Data;
				mCurBoundView = eBufferViewType::UAV;
			}

			//구조화버퍼 생성
			if (FAILED(GPUMgr::GetInst()->GetDevice()->CreateBuffer(&mBufferDesc, pData, mStructBuffer.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_A("Failed to create Structured Buffer!");
				return E_FAIL;
			}


			CreateSRV();
			CreateUAV();

			break;
		}

		}

		return S_OK;
	}

	void StructBuffer::SetData(void* _pData, size_t _uCount)
	{
		mElementCount = (UINT)_uCount;

		//g_arrSBufferShareData의 자신의 인덱스에 해당하는 위치에 이번에 업데이트된 구조체의 갯수를 삽입
		//상수 버퍼의 바인딩은 BindData()를 하는 시점에 해준다.
		SBufferCB cb = {};
		cb.SBufferDataCount = mElementCount;


		//생성 시 할당된 갯수보다 들어온 갯수가 더 클 경우 재할당하고, 거기에 데이터를 추가.
		//생성될 때 값을 지정할 수 있으므로 바로 return 해주면 될듯
		if (_uCount > mElementCapacity)
		{
			//다시 생성하고자 할때는 초기 데이터와 사이즈를 일치시켜서 생성해줘야 한다.
			Create(mElementStride, _uCount, _pData, _uCount);
			return;
		}


		auto pContext = GPUMgr::GetInst()->GetContext();
		switch (mSBufferDesc.eSBufferType)
		{
		case eStructBufferType::READ_ONLY:
		{
			{
				//Read Only일 경우 SBuffer에 바로 Map/UnMap을 해주면 된다.
				D3D11_MAPPED_SUBRESOURCE Data = {};
				if (SUCCEEDED(pContext->Map(mStructBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data)))
				{
					memcpy(Data.pData, _pData, (size_t)(mElementStride * _uCount));

					pContext->Unmap(mStructBuffer.Get(), 0);
				}

				break;
			}
		}

		case eStructBufferType::READ_WRITE:
		{
			{
				if (nullptr == mStagingBuffer.Get())
					CreateStagingBuffer();
				else
				{
					D3D11_BUFFER_DESC pDesc = {};
					mStagingBuffer->GetDesc(&pDesc);

					//생성되어 있는 Staging Buffer의 크기보다 클 경우 새로 생성
					if (pDesc.ByteWidth < mBufferDesc.ByteWidth)
						CreateStagingBuffer();
				}

				D3D11_MAPPED_SUBRESOURCE Data = {};
				if (SUCCEEDED(pContext->Map(mStagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data)))
				{
					memcpy(Data.pData, _pData, (size_t)(mElementStride * _uCount));
					pContext->Unmap(mStagingBuffer.Get(), 0);
					pContext->CopyResource(mStructBuffer.Get(), mStagingBuffer.Get());
				}
				break;
			}

		}

		}
	}

	void StructBuffer::GetData(void* _pDest, size_t _uDestByteCapacity)
	{
		auto pContext = GPUMgr::GetInst()->GetContext();

		switch (mSBufferDesc.eSBufferType)
		{
			memset(_pDest, 0, _uDestByteCapacity);
			return;
			//READ_ONLY일 경우에는 데이터 가져오기 불가능.
		case eStructBufferType::READ_ONLY:
		{
			//#ifdef _DEBUG
			//		D3D11_MAPPED_SUBRESOURCE Data = {};
			//
			//		pContext->Map(mStructBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);
			//
			//		size_t bytesize = mElementStride * mElementCount;
			//
			//		memcpy_s(_pDest, _uDestByteCapacity, Data.pData, bytesize);
			//
			//		pContext->Unmap(mStructBuffer.Get(), 0);
			//#endif
			break;
		}

		//읽기/쓰기 모두 가능한 Struct 버퍼일 경우에는 Staging 버퍼를 통해서 가져온다.
		case eStructBufferType::READ_WRITE:
		{
			if (nullptr == mStagingBuffer.Get())
				CreateStagingBuffer();
			else
			{
				D3D11_BUFFER_DESC pDesc = {};
				mStagingBuffer->GetDesc(&pDesc);

				//생성되어 있는 Staging Buffer의 크기보다 클 경우 새로 생성
				if (pDesc.ByteWidth < mBufferDesc.ByteWidth)
					CreateStagingBuffer();
			}

			pContext->CopyResource(mStagingBuffer.Get(), mStructBuffer.Get());

			D3D11_MAPPED_SUBRESOURCE Data = {};
			pContext->Map(mStagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

			size_t bytesize = mElementStride * mElementCapacity;
			memcpy_s(_pDest, _uDestByteCapacity, Data.pData, bytesize);

			pContext->Unmap(mStagingBuffer.Get(), 0);
			break;
		}

		default: break;
		}
	}

	void StructBuffer::BindDataSRV(int _SRVSlot, eShaderStageFlag_ _stageFlag)
	{
		UnBind();

		mCurBoundView = eBufferViewType::SRV;

		if (0 > _SRVSlot)
		{
			_SRVSlot = (int)mSBufferDesc.REGISLOT_t_SRV;
		}
		mCurBoundRegister = _SRVSlot;

		if (eShaderStageFlag::NONE == _stageFlag)
		{
			_stageFlag = mSBufferDesc.TargetStage;
		}

		

		//상수버퍼 바인딩
		BindConstBuffer(_stageFlag);

		auto pContext = GPUMgr::GetInst()->GetContext();
		if (eShaderStageFlag::VS & _stageFlag)
		{
			pContext->VSSetShaderResources(_SRVSlot, 1, mSRV.GetAddressOf());
		}

		if (eShaderStageFlag::HS & _stageFlag)
		{
			pContext->HSSetShaderResources(_SRVSlot, 1, mSRV.GetAddressOf());
		}

		if (eShaderStageFlag::DS & _stageFlag)
		{
			pContext->DSSetShaderResources(_SRVSlot, 1, mSRV.GetAddressOf());
		}

		if (eShaderStageFlag::GS & _stageFlag)
		{
			pContext->GSSetShaderResources(_SRVSlot, 1, mSRV.GetAddressOf());
		}

		if (eShaderStageFlag::PS & _stageFlag)
		{
			pContext->PSSetShaderResources(_SRVSlot, 1, mSRV.GetAddressOf());
		}

		if (eShaderStageFlag::CS & _stageFlag)
		{
			pContext->CSSetShaderResources(_SRVSlot, 1, mSRV.GetAddressOf());
		}
	}

	void StructBuffer::BindDataUAV(int _UAVSlot)
	{
		//읽기 쓰기 다 가능한 상태가 아닐경우 assert
		MH_ASSERT(eStructBufferType::READ_WRITE == mSBufferDesc.eSBufferType);

		UnBind();

		mCurBoundView = eBufferViewType::UAV;

		if (0 > _UAVSlot)
		{
			_UAVSlot = mSBufferDesc.REGISLOT_u_UAV;
		}
		mCurBoundRegister = _UAVSlot;

		//mSBufferDesc.TargetStage |= eShaderStageFlag::CS;
		BindConstBuffer(eShaderStageFlag::CS);

		UINT Offset = -1;
		GPUMgr::GetInst()->GetContext()->CSSetUnorderedAccessViews(_UAVSlot, 1, mUAV.GetAddressOf(), &Offset);
	}

	void StructBuffer::SetDefaultDesc()
	{
		switch (mSBufferDesc.eSBufferType)
		{
		case eStructBufferType::READ_ONLY:

			//CPU에서 구조화버퍼로 작성할 수 있어야 하므로 AccessFlag를 write로 설정
			mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			//CPU에서 버퍼를 작성하고 GPU에서 버퍼를 읽어들일 경우 사용
			mBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

			//SRV만 바인딩되도록 설정
			mBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			break;
		case eStructBufferType::READ_WRITE:

			//CPU는 직접 접근 불가, 
			mBufferDesc.CPUAccessFlags = 0;

			//GPU의 읽기/쓰기는 가능
			mBufferDesc.Usage = D3D11_USAGE_DEFAULT;

			//SRV, UAV 둘 다 바인딩되도록 설정
			mBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

			//READ_WRITE로 사용하겠다는 건 컴퓨트쉐이더를 사용하겠다는 의미 -> 실수 방지를 위해 플래그에 컴퓨트쉐이더 추가
			mSBufferDesc.TargetStage |= eShaderStageFlag::CS;

			break;
		default:
			break;
		}

		//기타 플래그에 구조화 버퍼로 설정값이 있음.
		mBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	}

	void StructBuffer::BindConstBuffer(eShaderStageFlag_ _stageFlag)
	{
		//구조체 정보를 담은 상수버퍼에 바인딩한 구조체 갯수를 넣어서 전달
		//상수버퍼의 주소는 한번 실행되면 변하지 않으므로 static, const 형태로 선언.
		static ConstBuffer* pStructCBuffer = RenderMgr::GetInst()->GetConstBuffer(eCBType::SBuffer);

		SBufferCB cb = {};
		cb.SBufferDataCount = mElementCount;

		pStructCBuffer->SetData(&cb);
		pStructCBuffer->BindData(_stageFlag);
	}



	bool StructBuffer::CreateStagingBuffer()
	{
		//SysMem <-> GPUMem 데이터 전송용 스테이징 버퍼 생성
		D3D11_BUFFER_DESC Desc = mBufferDesc;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		Desc.Usage = D3D11_USAGE_STAGING;
		Desc.BindFlags = 0;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		bool bResult = SUCCEEDED(GPUMgr::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, mStagingBuffer.ReleaseAndGetAddressOf()));

		if(false == bResult)
		{
			ERROR_MESSAGE_W(L"구조화 버퍼의 데이터를 주고받을 스테이징 버퍼 생성에 실패했습니다.");
		}
			
		return bResult;
	}

	bool StructBuffer::CreateSRV()
	{
		//SRV 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		SRVDesc.BufferEx.NumElements = mElementCapacity;

		bool bResult = SUCCEEDED(GPUMgr::GetInst()->GetDevice()->CreateShaderResourceView(mStructBuffer.Get(), &SRVDesc, mSRV.ReleaseAndGetAddressOf()));
		
		if (false == bResult)
		{
			ERROR_MESSAGE_W(L"구조화 버퍼의 Shader Resource View 생성에 실패했습니다.");

		}
		return bResult;
	}

	bool StructBuffer::CreateUAV()
	{
		//GPU에서 읽기 및 쓰기 작업이 가능해야 하므로 UAV 형태로 생성
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = mElementCapacity;

		bool bResult = SUCCEEDED(GPUMgr::GetInst()->GetDevice()->CreateUnorderedAccessView(mStructBuffer.Get(), &UAVDesc, mUAV.ReleaseAndGetAddressOf()));
		if (false == bResult)
		{
			ERROR_MESSAGE_W(L"구조화 버퍼의 Unordered Access View 생성에 실패했습니다.");

		}
		return bResult;
	}

	void StructBuffer::UnBind()
	{
		switch (mCurBoundView)
		{
		case mh::eBufferViewType::NONE:
			break;
		case mh::eBufferViewType::SRV:
		{

			auto pContext = GPUMgr::GetInst()->GetContext();

			ID3D11ShaderResourceView* pView = nullptr;
			if (eShaderStageFlag::VS & mSBufferDesc.TargetStage)
			{
				pContext->VSSetShaderResources(mCurBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::HS & mSBufferDesc.TargetStage)
			{
				pContext->HSSetShaderResources(mCurBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::DS & mSBufferDesc.TargetStage)
			{
				pContext->DSSetShaderResources(mCurBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::GS & mSBufferDesc.TargetStage)
			{
				pContext->GSSetShaderResources(mCurBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::PS & mSBufferDesc.TargetStage)
			{
				pContext->PSSetShaderResources(mCurBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::CS & mSBufferDesc.TargetStage)
			{
				pContext->CSSetShaderResources(mCurBoundRegister, 1, &pView);
			}

			break;
		}

		case mh::eBufferViewType::UAV:
		{
			static const UINT v2_Offset = -1;
			ID3D11UnorderedAccessView* pUAV = nullptr;
			GPUMgr::GetInst()->GetContext()->CSSetUnorderedAccessViews(mCurBoundRegister, 1, &pUAV, &v2_Offset);
			break;
		}

		default:
			ERROR_MESSAGE_W(L"알 수 없는 오류");
			break;
		}


		mCurBoundRegister = -1;
		mCurBoundView = eBufferViewType::NONE;
	}

}
