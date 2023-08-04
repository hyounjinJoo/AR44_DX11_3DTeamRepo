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
		bool Create(uint _dataCount = 1u);
		bool Create(size_t _dataSize, uint _dataCount = 1u);
		void SetData(void* _data, uint _dataCount = 1u);
		void BindData(eShaderStageFlag_ _stageFlag = eShaderStageFlag::NONE);

	private:
		const eCBType mType;


		uint				mDataSize;
		uint				mDataCount;
		eShaderStageFlag_	mPresetTargetStage;
	};


	template<typename T>
	inline bool ConstBuffer::Create(uint _dataCount)
	{
		return Create(sizeof(T), _dataCount);
	}
}
