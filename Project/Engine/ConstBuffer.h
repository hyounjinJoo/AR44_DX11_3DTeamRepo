#pragma once
#include "GPUBuffer.h"

namespace mh::GPU
{
	class ConstBuffer 
		: public GPUBuffer
	{
	public:
		ConstBuffer(eCBType _type);
		virtual ~ConstBuffer();

		bool Create(size_t _size);
		void SetData(void* _data);
		void Bind(eShaderStage _stage);

		//void SetType(eCBType type) { mType = type; }


	private:
		const eCBType mType;
	};
}
