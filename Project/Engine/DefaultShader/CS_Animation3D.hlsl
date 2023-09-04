#include "SH_Func_Animation3D.hlsli"
#include "SH_ConstBuffer.hlsli"
#include "SH_Resource.hlsli"



[numthreads(256, 1, 1)]
void main(int3 _threadID : SV_DispatchThreadID)
{
	//ThreadID.x = 본의 인덱스
	if (CB_Animation3D.BoneCount <= _threadID.x)
		return;

	float4 ZeroRot = float4(0.f, 0.f, 0.f, 1.f);
	matrix matBone = 0.f;

	int FrameIndex = _threadID.x * CB_Animation3D.FrameLength + CB_Animation3D.CurrentFrame;
	int FrameNextIndex = _threadID.x * CB_Animation3D.FrameLength + CB_Animation3D.NextFrame;

	float4 Scale = lerp(g_FrameTransArray[FrameIndex].Scale, g_FrameTransArray[FrameNextIndex].Scale, CB_Animation3D.FrameRatio);
	float4 Pos = lerp(g_FrameTransArray[FrameIndex].Pos, g_FrameTransArray[FrameNextIndex].Pos, CB_Animation3D.FrameRatio);
	float4 Rot = QuternionLerp(g_FrameTransArray[FrameIndex].RotQuat, g_FrameTransArray[FrameNextIndex].RotQuat, CB_Animation3D.FrameRatio);

	if (CB_Animation3D.bChangingAnim == TRUE)
	{
		uint ChangeFrameIndex = _threadID.x * CB_Animation3D.ChangeFrameLength * CB_Animation3D.ChangeFrameIdx;

		Scale = lerp(Scale, g_ChangeFrameTransArray[ChangeFrameIndex].Scale, CB_Animation3D.ChangeRatio);
		Pos = lerp(Pos, g_ChangeFrameTransArray[ChangeFrameIndex].Pos, CB_Animation3D.ChangeRatio);
		Rot = QuternionLerp(Rot, g_ChangeFrameTransArray[ChangeFrameIndex].RotQuat, CB_Animation3D.ChangeRatio);
	}

	MatrixAffineTransformation(Scale, ZeroRot, Rot, Pos, matBone);

	matrix matOffset = transpose(g_BoneOffsetArray[_threadID.x]);

	g_FinalBoneMatrixArrayRW[_threadID.x] = mul(matOffset, matBone);
	//g_BoneSocketMatrixArray[_threadID.x].matBone = transpose(matBone);

	//g_BoneSocketMatrixArray[_threadID.x].Scale = Scale.xyz;
	//g_BoneSocketMatrixArray[_threadID.x].Rot = Rot;
	//g_BoneSocketMatrixArray[_threadID.x].Pos = Pos.xyz;

	//g_InstancingBoneMatrixArray[CB_Animation3D.RowIndex * CB_Animation3D.BoneCount + _threadID.x] = mul(matOffset, matBone);
}
