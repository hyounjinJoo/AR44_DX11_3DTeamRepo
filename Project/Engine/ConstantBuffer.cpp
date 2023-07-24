#include "EnginePCH.h"

#include "ConstantBuffer.h"
#include "GraphicDevice_DX11.h"

namespace mh::graphics
{
	ConstantBuffer::ConstantBuffer(eCBType _type)
		: mType(_type)
	{

	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(size_t _size)
	{
		// 상수 버퍼
		desc.ByteWidth = static_cast<UINT>(_size);
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		if (!GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()))
			return false;

		return true;
	}

	void ConstantBuffer::SetData(void* _data)
	{
		GetDevice()->SetData(buffer.Get(), _data, desc.ByteWidth);
	}

	void ConstantBuffer::Bind(eShaderStage _stage)
	{
		if (_stage == eShaderStage::ALL)
		{
			GetDevice()->BindConstantBuffer(eShaderStage::VS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::HS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::DS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::GS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::PS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::CS, mType, buffer.Get());
		}
		else
		{
			GetDevice()->BindConstantBuffer(_stage, mType, buffer.Get());
		}
	}
}
