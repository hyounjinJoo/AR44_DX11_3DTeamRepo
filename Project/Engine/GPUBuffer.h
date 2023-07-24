#pragma once
#include "Entity.h"

#include "Graphics.h"

namespace mh::graphics
{
	class GPUBuffer
		: public Entity
	{
	public:
		GPUBuffer() = default;
		virtual ~GPUBuffer() = default;

		enum class eType
		{
			Buffer,
			Texture,
			UnknownType,
		} type = eType::UnknownType;

		D3D11_BUFFER_DESC desc;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

}


