#pragma once
#include "ComputeShader.h"
#include "StructBuffer.h"

namespace mh
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		~ParticleShader();

		virtual bool Binds() override;
		virtual void Clear() override;

		void SetStrcutedBuffer(StructBuffer* _buffer) { mBuffer = _buffer; }
		void SetSharedStrutedBuffer(StructBuffer* _buffer) { mSharedBuffer = _buffer; }

	private:
		StructBuffer* mBuffer;
		StructBuffer* mSharedBuffer;
	};
}
