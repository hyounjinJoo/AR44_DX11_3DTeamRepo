#include "SH_Func_Animation3D.hlsli"
#include "SH_ConstBuffer.hlsli"
#include "SH_Resource.hlsli"

[numthreads(256, 1, 1)]
void main(int3 _iThreadIdx : SV_DispatchThreadID)
{
	//Total Data Count.x = Bone Count
	if (CB_ComputeShader.TotalDataCount.x <= (uint)_iThreadIdx.x)
		return;

    // 오프셋 행렬을 곱하여 최종 본행렬을 만들어낸다.		
	float4 vQZero = float4(0.f, 0.f, 0.f, 1.f);
	matrix matBone = (matrix) 0.f;

    // Frame Data Index == Bone Count * Frame Count + _iThreadIdx.x
	uint iFrameDataIndex = CB_ComputeShader.TotalDataCount.x * CB_Animation3D.CurFrameIdx + _iThreadIdx.x;
	uint iNextFrameDataIdx = CB_ComputeShader.TotalDataCount.x * (CB_Animation3D.CurFrameIdx + 1) + _iThreadIdx.x;

	float4 vScale = lerp(g_arrFrameTrans[iFrameDataIndex].vScale, g_arrFrameTrans[iNextFrameDataIdx].vScale, CB_Animation3D.FrameRatio);
	float4 vTrans = lerp(g_arrFrameTrans[iFrameDataIndex].vTranslate, g_arrFrameTrans[iNextFrameDataIdx].vTranslate, CB_Animation3D.FrameRatio);
	float4 qRot = QuternionLerp(g_arrFrameTrans[iFrameDataIndex].qRot, g_arrFrameTrans[iNextFrameDataIdx].qRot, CB_Animation3D.FrameRatio);

    // 최종 본행렬 연산
	MatrixAffineTransformation(vScale, vQZero, qRot, vTrans, matBone);

    // 최종 본행렬 연산    
    //MatrixAffineTransformation(g_arrFrameTrans[iFrameDataIndex].vScale, vQZero, g_arrFrameTrans[iFrameDataIndex].qRot, g_arrFrameTrans[iFrameDataIndex].vTranslate, matBone);

	matrix matOffset = transpose(g_arrOffset[_iThreadIdx.x]);

    // 구조화버퍼에 결과값 저장
	g_arrFinalMat[_iThreadIdx.x] = mul(matOffset, matBone);
}
