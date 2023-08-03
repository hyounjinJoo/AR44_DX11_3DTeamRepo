#pragma once
#include "Entity.h"

#include "define_GPU.h"

namespace mh
{
	enum class eBufferType
	{
		Const,
		Struct,
		UnknownType,
	};

	class GPUBuffer
		: public Entity
	{
	public:
		GPUBuffer(eBufferType _Type);
		virtual ~GPUBuffer() = default;

	protected:
		ComPtr<ID3D11Buffer>	mBuffer;
		D3D11_BUFFER_DESC		mBufferDesc;

	private:
		eBufferType mBufferType;
	};

}


