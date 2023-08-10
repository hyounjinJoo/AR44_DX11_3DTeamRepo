#pragma once
#include "GPUBuffer.h"

namespace mh
{
	class ConstBuffer 
		: public GPUBuffer
	{
	public:
		ConstBuffer(define::eCBType _type);
		virtual ~ConstBuffer();

		template <typename T>
		bool Create(uint _dataCount = 1u);
		bool Create(size_t _dataSize, uint _dataCount = 1u);
		void SetData(void* _data, uint _dataCount = 1u);
		void BindData(define::eShaderStageFlag_ _stageFlag = define::eShaderStageFlag::NONE);

		void SetPresetTargetStage(define::eShaderStageFlag_ _flag) { mPresetTargetStage = _flag; }

	private:
		const define::eCBType mType;


		uint				mDataSize;
		uint				mDataCount;
		define::eShaderStageFlag_	mPresetTargetStage;
	};


	template<typename T>
	inline bool ConstBuffer::Create(uint _dataCount)
	{
		return Create(sizeof(T), _dataCount);
	}
}
