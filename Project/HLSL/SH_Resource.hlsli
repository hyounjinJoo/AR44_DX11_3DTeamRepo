#ifndef SH_RESOURCE
#define SH_RESOURCE

#include "SH_CommonStruct.hlsli"

//t 버퍼

//Material Default Texture
TEXTURE2D(Tex_0, t, 0);
TEXTURE2D(Tex_1, t, 1);
TEXTURE2D(Tex_2, t, 2);
TEXTURE2D(Tex_3, t, 3);
TEXTURE2D(Tex_4, t, 4);
TEXTURE2D(Tex_5, t, 5);
TEXTURE2D(Tex_6, t, 6);
TEXTURE2D(Tex_7, t, 7);


//3D Material
TEXTURE2D(AlbedoTexture, t, 0);
TEXTURE2D(NormalTexture, t, 1);
TEXTURE2D(SpecularTexture, t, 2);
TEXTURE2D(EmissiveTexture, t, 3);
TEXTURE2D(RoughnessMetalicTexture, t, 4);
#define IsAlbedoTex bTex_0
#define IsNormalTex bTex_1
#define IsSpecularTex bTex_2
#define IsEmissiveTex bTex_3
#define IsRMTTex bTex_4


//Deffered + Light MRT
TEXTURE2D(AlbedoTarget, t, 0);
TEXTURE2D(NormalTarget, t, 1);
TEXTURE2D(SpecularTarget, t, 2);
TEXTURE2D(EmissiveTarget, t, 3);
TEXTURE2D(RoughnessMetalicTarget, t, 4);
TEXTURE2D(PositionTarget, t, 5);
TEXTURE2D(DiffuseLightTarget, t, 6);
TEXTURE2D(SpecularLightTarget, t, 7);


//Bone Matrices(Animation 3D)
//현재 재생중인 애니메이션의 키프레임 데이터
SBUFFER(g_FrameTransArray, tAnimKeyframeTranslation, t, 16);
//(애니메이션 변경중일 경우) 다음 애니메이션의 키프레임 데이터
SBUFFER(g_ChangeFrameTransArray, tAnimKeyframeTranslation, t, 17);
SBUFFER(g_BoneOffsetArray, Matrix, t, 18);

//최종 행렬이 저장되는 구조화버퍼
SBUFFER(g_FinalBoneMatrixArray, Matrix, t, 19);
SBUFFER_RW(g_FinalBoneMatrixArrayRW, Matrix, u, 0);

//SBUFFER(g_InstancingBoneMatrixArray, Matrix, t, 18);

SBUFFER_RW(g_BoneSocketMatrixArray, tOutputBoneInfo, u, 1);
SBUFFER_RW(g_InstancingBoneMatrixArray, Matrix, u, 2);


// Light
SBUFFER(lightAttributes, tLightAttribute, t, 14);

//StructuredBuffer<LightAttribute> lightAttributes : register(t13);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

// noise
TEXTURE2D(NoiseTexture, t, 16);

// postProcess
TEXTURE2D(postProcessTexture, t, 60);
TEXTURE2D(guiGameTexture, t, 61);

// cube
//TEXTURECUBE(skyBoxCube, t, 8);


#endif
