#include "EnginePCH.h"

#include "ConstBuffer.h"
#include "GPUMgr.h"

namespace mh
{
	ConstBuffer::ConstBuffer(eCBType _type)
		: GPUBuffer(eBufferType::Const)
		, mType(_type)
		, mDataSize()
		, mDataCount()
		, mPresetTargetStage(eShaderStageFlag::ALL)
	{

	}

	ConstBuffer::~ConstBuffer()
	{
	}

	bool ConstBuffer::Create(size_t _dataSize, UINT _dataCount)
	{
		mDataSize = (UINT)_dataSize;
		mDataCount = _dataCount;

		// 상수 버퍼
		mBufferDesc.ByteWidth = mDataSize * _dataCount;
		mBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		mBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mBufferDesc.MiscFlags = 0;
		mBufferDesc.StructureByteStride = 0;

		bool bResult = SUCCEEDED(GPUMgr::GetInst()->GetDevice()->CreateBuffer(&mBufferDesc, nullptr, mBuffer.GetAddressOf()));
			
		if (false == bResult)
		{
			mBufferDesc = {};
			mBuffer = nullptr;
		}

		return bResult;
	}

	void ConstBuffer::SetData(void* _data, UINT _dataCount)
	{
		MH_ASSERT(nullptr != _data && _dataCount <= mDataCount);
	

		auto pContext = GPUMgr::GetInst()->GetContext();
		D3D11_MAPPED_SUBRESOURCE tSubRes{};

		if (SUCCEEDED(pContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
		{
			memcpy(tSubRes.pData, _data, mDataSize * _dataCount);
			pContext->Unmap(mBuffer.Get(), 0);
		}
	}

	void ConstBuffer::BindData(eShaderStageFlag_ _stageFlag)
	{
		if (eShaderStageFlag::NONE == _stageFlag)
		{
			_stageFlag = mPresetTargetStage;
		}

		auto pContext = GPUMgr::GetInst()->GetContext();
		if (eShaderStageFlag::VS & _stageFlag)
		{
			pContext->VSSetConstantBuffers((UINT)mType, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::HS & _stageFlag)
		{
			pContext->HSSetConstantBuffers((UINT)mType, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::DS & _stageFlag)
		{
			pContext->DSSetConstantBuffers((UINT)mType, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::GS & _stageFlag)
		{
			pContext->GSSetConstantBuffers((UINT)mType, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::PS & _stageFlag)
		{
			pContext->PSSetConstantBuffers((UINT)mType, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::CS & _stageFlag)
		{
			pContext->CSSetConstantBuffers((UINT)mType, 1u, mBuffer.GetAddressOf());
		}
	}
}
