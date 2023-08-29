#include "SH_Func_Animation3D.hlsli"
#include "SH_ConstBuffer.hlsli"
#include "SH_Resource.hlsli"



[numthreads(256, 1, 1)]
void main(int3 _threadID : SV_DispatchThreadID)
{
	if (CB_Animation3D.BoneCount <= _threadID.x)
		return;

	float4 ZeroRot = float4(0.f, 0.f, 0.f, 1.f);
	matrix matBone = (matrix) 0.f;

	uint FrameIndex = _threadID.x * CB_Animation3D.FrameCount + CB_Animation3D.CurrentFrame;
	uint FrameNextIndex = _threadID.x * CB_Animation3D.FrameCount + CB_Animation3D.NextFrame;

	float4 Scale = lerp(g_FrameTransArray[FrameIndex].vScale, g_FrameTransArray[FrameNextIndex].vScale, CB_Animation3D.FrameRatio);
	float4 Pos = lerp(g_FrameTransArray[FrameIndex].vTranslate, g_FrameTransArray[FrameNextIndex].vTranslate, CB_Animation3D.FrameRatio);
	float4 Rot = QuternionLerp(g_FrameTransArray[FrameIndex].qRot, g_FrameTransArray[FrameNextIndex].qRot, CB_Animation3D.FrameRatio);

	if (CB_Animation3D.bChangingAnim == TRUE)
	{
		uint ChangeFrameIndex = _threadID.x * CB_Animation3D.ChangeFrameCount;

		Scale = lerp(Scale, g_ChangeFrameTransArray[ChangeFrameIndex].vScale, CB_Animation3D.ChangeRatio);
		Pos = lerp(Pos, g_ChangeFrameTransArray[ChangeFrameIndex].vTranslate, CB_Animation3D.ChangeRatio);
		Rot = QuternionLerp(Rot, g_ChangeFrameTransArray[ChangeFrameIndex].qRot, CB_Animation3D.ChangeRatio);
	}

	MatrixAffineTransformation(Scale, ZeroRot, Rot, Pos, matBone);

	matrix matOffset = transpose(g_OffsetArray[_threadID.x]);

	g_FinalBoneMatrixArray[_threadID.x] = mul(matOffset, matBone);
	//g_BoneSocketMatrixArray[_threadID.x].matBone = transpose(matBone);

	//g_BoneSocketMatrixArray[_threadID.x].Scale = Scale.xyz;
	//g_BoneSocketMatrixArray[_threadID.x].Rot = Rot;
	//g_BoneSocketMatrixArray[_threadID.x].Pos = Pos.xyz;

	//g_InstancingBoneMatrixArray[CB_Animation3D.RowIndex * CB_Animation3D.BoneCount + _threadID.x] = mul(matOffset, matBone);
}
