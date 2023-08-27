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
		virtual void Clear();

		void SetBoneCount(int _iBoneCount) { mAnim3DData.g_AnimBoneCount = _iBoneCount; }
		void SetFrameIndex(int _iFrameIdx) { mAnim3DData.g_AnimCurrentFrame = _iFrameIdx; }
		void SetNextFrameIdx(int _iFrameIdx) { mAnim3DData.g_AnimNextFrame = _iFrameIdx; }
		void SetFrameRatio(float _fFrameRatio) { mAnim3DData.g_AnimRatio = _fFrameRatio; }
		void SetFrameDataBuffer(StructBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
		void SetOffsetMatBuffer(StructBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
		void SetOutputBuffer(StructBuffer* _buffer) { m_pOutputBuffer = _buffer; }

	private:
		tCB_Animation3D mAnim3DData;

		StructBuffer* m_pFrameDataBuffer;			// t13
		StructBuffer* m_pOffsetMatBuffer;			// t14 
		StructBuffer* m_pOutputBuffer;				// u0
    };
}


