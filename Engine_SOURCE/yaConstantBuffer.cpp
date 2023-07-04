#include "yaConstantBuffer.h"
#include "yaGraphicDevice_DX11.h"


namespace ya::graphics
{
	ConstantBuffer::ConstantBuffer(eCBType type)
		: mType(type)
	{

	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(size_t size)
	{
		// 상수 버퍼
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		if (!GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()))
			return false;

		return true;
	}

	void ConstantBuffer::SetData(void* data)
	{
		GetDevice()->SetData(buffer.Get(), data, desc.ByteWidth);
	}

	void ConstantBuffer::Bind(eShaderStage stage)
	{
		if (stage == eShaderStage::ALL)
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
			GetDevice()->BindConstantBuffer(stage, mType, buffer.Get());
		}
	}
}