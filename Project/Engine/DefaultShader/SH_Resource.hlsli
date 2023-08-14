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


// defferd
TEXTURE2D(positionTarget, t, 4);
TEXTURE2D(normalTarget, t, 5);
TEXTURE2D(albedoTarget, t, 6);
TEXTURE2D(specularTarget, t, 7);
//Texture2D positionTarget : register(t4);
//Texture2D normalTarget : register(t5);
//Texture2D albedoTarget : register(t6);
//Texture2D specularTarget : register(t7);


// Light
TEXTURE2D(diffuseLightTarget, t, 8);
TEXTURE2D(specularLightTarget, t, 9);
//Texture2D diffuseLightTarget : register(t8);
//Texture2D specularLightTarget : register(t9);


//Atlas texture(Animation 2D)
TEXTURE2D(atlasTexture, t, 12);

// Light
SBUFFER(lightAttributes, tLightAttribute, t, 13);


//StructuredBuffer<LightAttribute> lightAttributes : register(t13);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);




// noise
TEXTURE2D(NoiseTexture, t, 16);



// postProcess
TEXTURE2D(postProcessTexture, t, 60);
TEXTURE2D(guiGameTexture, t, 61);


#endif
