#include "EnginePCH.h"

#include "ConstBuffer.h"
#include "GPUMgr.h"

namespace mh
{
	ConstBuffer::ConstBuffer(eCBType _type)
		: mType(_type)
	{

	}

	ConstBuffer::~ConstBuffer()
	{
	}

	bool ConstBuffer::Create(size_t _size)
	{
		// 상수 버퍼
		desc.ByteWidth = static_cast<UINT>(_size);
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		if (!GPUMgr::GetInst()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()))
			return false;

		return true;
	}

	void ConstBuffer::SetData(void* _data)
	{
		GPUMgr::GetInst()->SetData(buffer.Get(), _data, desc.ByteWidth);
	}

	void ConstBuffer::Bind(eShaderStage _stage)
	{
		if (_stage == eShaderStage::ALL)
		{
			GPUMgr::GetInst()->BindConstBuffer(eShaderStage::VS, mType, buffer.Get());
			GPUMgr::GetInst()->BindConstBuffer(eShaderStage::HS, mType, buffer.Get());
			GPUMgr::GetInst()->BindConstBuffer(eShaderStage::DS, mType, buffer.Get());
			GPUMgr::GetInst()->BindConstBuffer(eShaderStage::GS, mType, buffer.Get());
			GPUMgr::GetInst()->BindConstBuffer(eShaderStage::PS, mType, buffer.Get());
			GPUMgr::GetInst()->BindConstBuffer(eShaderStage::CS, mType, buffer.Get());
		}
		else
		{
			GPUMgr::GetInst()->BindConstBuffer(_stage, mType, buffer.Get());
		}
	}
}
