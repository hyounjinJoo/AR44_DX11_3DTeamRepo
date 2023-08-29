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
#define IsAlbedoTex bTex_0
#define IsNormalTex bTex_1
#define IsSpecularTex bTex_2
#define IsEmissiveTex bTex_3


//Deffered + Light MRT
TEXTURE2D(AlbedoTarget, t, 0);
TEXTURE2D(NormalTarget, t, 1);
TEXTURE2D(SpecularTarget, t, 2);
TEXTURE2D(EmissiveTarget, t, 3);
TEXTURE2D(PositionTarget, t, 4);
TEXTURE2D(DiffuseLightTarget, t, 5);
TEXTURE2D(SpecularLightTarget, t, 6);


//Bone Matrices(Animation 3D)
SBUFFER(g_arrFrameTrans, tFrameTranslation, t, 16);
SBUFFER(g_arrOffset, Matrix, t, 17);
SBUFFER_RW(g_arrFinalMat, Matrix, u, 0);


//StructuredBuffer<tFrameTranslation> g_arrFrameTrans : register(t16);
//StructuredBuffer<matrix> g_arrOffset : register(t17);
//RWStructuredBuffer<matrix> g_arrFinalMat : register(u0);

SBUFFER(g_arrBoneMat, Matrix, t, 30);


// Light
SBUFFER(lightAttributes, tLightAttribute, t, 14);


//StructuredBuffer<LightAttribute> lightAttributes : register(t13);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);




// noise
TEXTURE2D(NoiseTexture, t, 16);



// postProcess
TEXTURE2D(postProcessTexture, t, 60);
TEXTURE2D(guiGameTexture, t, 61);


#endif
