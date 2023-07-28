#pragma once
#include "GPUBuffer.h"

namespace mh
{
	class ConstBuffer 
		: public GPUBuffer
	{
	public:
		ConstBuffer(eCBType _type);
		virtual ~ConstBuffer();

		template <typename T>
		bool Create(UINT _dataCount = 1u);
		bool Create(size_t _dataSize, UINT _dataCount = 1u);
		void SetData(void* _data, UINT _dataSize = 1u);
		void BindData(eShaderStageFlag_ _stageFlag = eShaderStageFlag::NONE);

	private:
		const eCBType mType;

		UINT				mDataSize;
		UINT				mDataCount;
		eShaderStageFlag_	mPresetTargetStage;
	};


	template<typename T>
	inline bool ConstBuffer::Create(UINT _dataCount)
	{
		return Create(sizeof(T), _dataCount);
	}
}
