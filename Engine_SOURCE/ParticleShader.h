#pragma once
#include "ComputeShader.h"
#include "StructedBuffer.h"

namespace mh::graphics
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		~ParticleShader();

		virtual void Binds() override;
		virtual void Clear() override;

		void SetStrcutedBuffer(StructedBuffer* _buffer) { mBuffer = _buffer; }
		void SetSharedStrutedBuffer(StructedBuffer* _buffer) { mSharedBuffer = _buffer; }

	private:
		StructedBuffer* mBuffer;
		StructedBuffer* mSharedBuffer;
	};
}