#ifndef SH_RESOURCE
#define SH_RESOURCE

#include "SH_CommonStruct.hlsli"



//t 버퍼
TEXTURE2D(albedoTexture, t, 0);
TEXTURE2D(normalTexture, t, 1);
TEXTURE2D(specularTexture, t, 2);
TEXTURE2D(emissiveTexture, t, 3);
// Material Default Texture
//Texture2D albedoTexture : register(t0);
//Texture2D normalTexture : register(t1);
//Texture2D specularTexture : register(t2);
//Texture2D emissiveTexture : register(t3);


//defferd
TEXTURE2D(albedoTarget, t, 0);
TEXTURE2D(normalTarget, t, 1);
TEXTURE2D(specularTarget, t, 2);
TEXTURE2D(emissiveTarget, t, 3);
TEXTURE2D(positionTarget, t, 4);

// Light
TEXTURE2D(diffuseLightTarget, t, 5);
TEXTURE2D(specularLightTarget, t, 6);


//Atlas texture(Animation 2D)
TEXTURE2D(atlasTexture, t, 11);

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
