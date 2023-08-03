#include "EnginePCH.h"

#include "GPUBuffer.h"
#include "GPUMgr.h"

namespace mh
{
	GPUBuffer::GPUBuffer(eBufferType _Type)
		: mBuffer()
		, mBufferDesc{}
		, mBufferType(_Type)
	{
	}
}

