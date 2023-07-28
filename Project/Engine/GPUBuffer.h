#pragma once
#include "Entity.h"

#include "define_GPU.h"

namespace mh
{

	class GPUBuffer
		: public Entity
	{
	public:
		GPUBuffer() = default;
		virtual ~GPUBuffer() = default;

	public:
		const D3D11_BUFFER_DESC& GetDesc() const { return mDesc; }
		ComPtr<ID3D11Buffer> GetBuffer() { return mBuffer; }


	protected:
		enum class eType
		{
			Buffer,
			Texture,
			UnknownType,
		} type = eType::UnknownType;

		D3D11_BUFFER_DESC mDesc;
		ComPtr<ID3D11Buffer> mBuffer;
	};

}


