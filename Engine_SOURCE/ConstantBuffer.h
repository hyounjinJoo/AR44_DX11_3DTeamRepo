#pragma once
#include "Graphics.h"

namespace mh::graphics
{
	class ConstantBuffer : GpuBuffer
	{
	public:
		ConstantBuffer(eCBType _type);
		virtual ~ConstantBuffer();

		bool Create(size_t _size);
		void SetData(void* _data);
		void Bind(eShaderStage _stage);

		//void SetType(eCBType type) { mType = type; }


	private:
		const eCBType mType;
	};
}
