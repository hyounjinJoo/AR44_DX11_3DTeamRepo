#include "EnginePCH.h"
#include "StructBuffer.h"

#include "GPUMgr.h"

namespace mh
{
	StructBuffer::StructBuffer()
		: mSRV(nullptr)
		, mType(eSRVType::SRV)
		, mSize(0)
		, mStride(0)
		, mSRVSlot(0)
		, mUAVSlot(0)
		, mWriteBuffer(nullptr)
		, mReadBuffer(nullptr)
	{
		
		
	}

	StructBuffer::~StructBuffer()
	{

	}

	bool StructBuffer::Create(UINT _size, UINT _stride, eSRVType _type, void* _data, bool _cpuAccess)
	{
		mType = _type;
		mSize = _size;
		mStride = _stride;


		setDiscription();
		createBuffer(_data);
		createView();

		if (_cpuAccess)
		{
			createRWBuffer();
		}

		return true;
	}

	void StructBuffer::SetData(void* data, size_t bufferCount)
	{
		UINT count = (UINT)bufferCount;
		if (mStride < count)
		{
			Create(mSize, count, eSRVType::SRV, data);
		}
		else
		{
			GPUMgr::GetInst()->SetData(mWriteBuffer.Get(), data, mSize * count);
		}
		GPUMgr::GetInst()->CopyResource(buffer.Get(), mWriteBuffer.Get());
	}

	void StructBuffer::GetData(void* data, UINT size)
	{
		GPUMgr::GetInst()->CopyResource(mReadBuffer.Get(), buffer.Get());

		//read buffer -> systemmemory
		if (size == 0)
		{
			GPUMgr::GetInst()->SetData(mReadBuffer.Get(), data, mSize * mStride);
		}
		else
		{
			GPUMgr::GetInst()->SetData(mReadBuffer.Get(), data, size);
		}
	}

	void StructBuffer::BindSRV(eShaderStage stage, UINT slot)
	{
		mSRVSlot = slot;

		GPUMgr::GetInst()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
	}

	void StructBuffer::BindUAV(eShaderStage stage, UINT slot)
	{
		mUAVSlot = slot;
		UINT i = -1;
		GPUMgr::GetInst()->BindUnorderdAccessView(slot, 1, mUAV.GetAddressOf(), &i);
	}

	void StructBuffer::Clear()
	{
		ID3D11ShaderResourceView* srv = nullptr;
		GPUMgr::GetInst()->BindShaderResource(eShaderStage::VS, mSRVSlot, &srv);
		GPUMgr::GetInst()->BindShaderResource(eShaderStage::HS, mSRVSlot, &srv);
		GPUMgr::GetInst()->BindShaderResource(eShaderStage::DS, mSRVSlot, &srv);
		GPUMgr::GetInst()->BindShaderResource(eShaderStage::GS, mSRVSlot, &srv);
		GPUMgr::GetInst()->BindShaderResource(eShaderStage::PS, mSRVSlot, &srv);
		GPUMgr::GetInst()->BindShaderResource(eShaderStage::CS, mSRVSlot, &srv);

		ID3D11UnorderedAccessView* uav = nullptr;
		UINT i = -1;
		GPUMgr::GetInst()->BindUnorderdAccessView(mUAVSlot, 1, &uav, &i);
	}

	void StructBuffer::setDiscription()
	{
		desc.ByteWidth = mSize * mStride;
		desc.StructureByteStride = mSize;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		if (mType == eSRVType::UAV)
		{
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE
				| D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
		}
		else if (mType == eSRVType::SRV)
		{
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		}
	}

	bool StructBuffer::createBuffer(void* data)
	{
		if (data)
		{
			D3D11_SUBRESOURCE_DATA tSub = {};
			tSub.pSysMem = data;

			if (!(GPUMgr::GetInst()->CreateBuffer(&desc, &tSub, buffer.GetAddressOf())))
				return false;
		}
		else
		{
			if (!(GPUMgr::GetInst()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf())))
				return false;
		}

		return true;
	}

	bool StructBuffer::createView()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.BufferEx.NumElements = mStride;
		srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;

		if (!(GPUMgr::GetInst()->CreateShaderResourceView(buffer.Get(), &srvDesc, mSRV.GetAddressOf())))
			return false;

		if (mType == eSRVType::UAV)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Buffer.NumElements = mStride;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

			if (!(GPUMgr::GetInst()->CreateUnorderedAccessView(buffer.Get(), &uavDesc, mUAV.GetAddressOf())))
				return false;
		}

		return false;
	}

	bool StructBuffer::createRWBuffer()
	{
		//mWriteBuffer(nullptr)
		//mReadBuffer(nullptr)

		D3D11_BUFFER_DESC wDesc = { desc };
		wDesc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		wDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		wDesc.Usage = D3D11_USAGE_DYNAMIC;
		wDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (!(GPUMgr::GetInst()->CreateBuffer(&wDesc, nullptr, mWriteBuffer.GetAddressOf())))
			return false;

		D3D11_BUFFER_DESC rDesc = { desc };
		rDesc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		rDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		rDesc.Usage = D3D11_USAGE_DEFAULT;
		rDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		if (!(GPUMgr::GetInst()->CreateBuffer(&rDesc, nullptr, mReadBuffer.GetAddressOf())))
			return false;

		return true;
	}

}
