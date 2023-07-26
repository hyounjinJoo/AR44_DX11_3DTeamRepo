#pragma once

#include "GPUBuffer.h"

namespace mh::GPU
{
	class StructBuffer 
		: public GPUBuffer
	{
	public:
		StructBuffer();
		virtual ~StructBuffer();

		bool Create(UINT _size, UINT _stride, eSRVType _type, void* _data, bool _cpuAccess = false);
		void SetData(void* _data, UINT _bufferCount);
		void GetData(void* _data, UINT _size = 0);
		void BindSRV(eShaderStage _stage, UINT _slot);
		void BindUAV(eShaderStage _stage, UINT _slot);

		void Clear();
		
		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

	private:
		void setDiscription();
		bool createBuffer(void* _data);
		bool createView();
		bool createRWBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer>				mWriteBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>				mReadBuffer;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	mUAV;
		eSRVType mType;

		UINT mSize;
		UINT mStride;

		UINT mSRVSlot;
		UINT mUAVSlot;
	};
}
