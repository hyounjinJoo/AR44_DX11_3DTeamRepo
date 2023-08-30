#pragma once
#include "ComputeShader.h"
#include "DefaultShader/SH_CommonStruct.hlsli"

namespace mh
{


	class StructBuffer;
    class Animation3DShader :
        public ComputeShader
    {
	public:
		Animation3DShader();
		virtual ~Animation3DShader();

		virtual bool BindData();
		virtual void UnBindData();

		void SetBoneCount(int _iBoneCount) { mAnim3DData.BoneCount = _iBoneCount; }
		void SetCurFrameIdx(int _iFrameIdx) { mAnim3DData.CurrentFrame = _iFrameIdx; }
		void SetNextFrameIdx(int _iFrameIdx) { mAnim3DData.NextFrame = _iFrameIdx; }
		void SetFrameRatio(float _fFrameRatio) { mAnim3DData.FrameRatio = _fFrameRatio; }
		void SetFrameDataBuffer(StructBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
		void SetFrameLength(int _frameLength) { mAnim3DData.FrameLength = _frameLength; }
		void SetOffsetMatBuffer(StructBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
		void SetOutputBuffer(StructBuffer* _buffer) { m_pOutputBuffer = _buffer; }

	private:
		tCB_Animation3D mAnim3DData;

		StructBuffer* m_pFrameDataBuffer;
		StructBuffer* m_pOffsetMatBuffer;
		StructBuffer* m_pOutputBuffer;
    };
}


