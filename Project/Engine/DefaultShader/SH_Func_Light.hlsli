#ifndef SH_FUNC_LIGHT
#define SH_FUNC_LIGHT


#ifndef __cplusplus//HLSL
#include "SH_Globals.hlsli"
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

//2D
void CalculateLight(in out tLightColor pLightColor, float3 position, int idx)
{
	if (0 == lightAttributes[idx].lightType)
	{
		pLightColor.diffuse += lightAttributes[idx].color.diffuse;
	}
	else if (1 == lightAttributes[idx].lightType)
	{
		float length = distance(lightAttributes[idx].position.xy, position.xy);
        
		if (length < lightAttributes[idx].radius)
		{
			float ratio = 1.0f - (length / lightAttributes[idx].radius);
			pLightColor.diffuse += lightAttributes[idx].color.diffuse * ratio;
		}
        
	}
	else
	{
        
	}
}

//3D
static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

void CalculateLight3D(float3 viewPos, float3 viewNormal, int lightIdx, inout tLightColor lightColor)
{
   
	tLightAttribute lightInfo = lightAttributes[lightIdx];
    
    //view space 상에서 빛의 세기를 구함
	float3 LightDir = (float3) 0.0f;
 
	
	float fDiffPow = 0.f;//전체 사용
	float fDistPow = 1.f;//PointLight, SpotLight에 사용
	float fAnglePow = 1.f;//SpotLight에 사용
    
    // Directional
	if (0 == lightInfo.lightType)
	{
        // Light 의 ViewSpace 에서의 '방향'(w값 0)
		LightDir = mul(float4(lightInfo.position.xyz, 0.f), CB_Transform.view).xyz;
	}
    // point
	else if (1 == lightInfo.lightType)
	{
		// Light 의 ViewSpace 에서의 '위치'(w값 1)
		float4 LightViewPos = mul(float4(lightInfo.position.xyz, 1.f), CB_Transform.view);
       
        // 표면위치 - 광원 위치 하면 방향 벡터를 구할 수 있음. 이걸 정규화
		LightDir = normalize(viewPos - LightViewPos.xyz);
        
        // 거리에 따른 세기 변화
		float fDist = distance(viewPos, LightViewPos.xyz);
		
		//노멀 <-> 광원 사이의 거리 / 빛의 범위 한 값을 0~1 사이로 제한
		fDistPow = 1.f - saturate(fDist / lightInfo.radius);
	}
	//SpotLight
	else
	{
        
	}
	
	//빛과 노멀 벡터 사이의 cos값
	float cosLight = dot(-LightDir, viewNormal);
	
	// Diffuse Power
	float fPow = saturate(cosLight);
    
    // Specular 계산
	float3 vViewReflect = normalize(LightDir + 2.f * cosLight * viewNormal);
    
    // 카메라에서 픽셀 지점을 바라보는 시선 벡터
	float3 vEye = -normalize(viewPos);
    
    // 반사광 세기          
	float fRelfectPow = pow(saturate(dot(vViewReflect, vEye)), 10);
	
	lightColor.diffuse += lightInfo.color.diffuse * fPow * fDistPow;
	lightColor.specular += lightInfo.color.specular * fRelfectPow * fDistPow;
	lightColor.ambient += lightInfo.color.ambient;
}

//void CalculateLight3D(float3 _vViewPos, float3 _vViewNormal, int _LightIdx, inout tLightColor _lightcolor)
//{
//    LightAttribute lightinfo = lightAttributes[_LightIdx];
       
//    float3 ViewLightDir = (float3) 0.f;
//    float fDiffPow = (float) 0.f;
//    float fSpecPow = (float) 0.f;
    
//    // Directional Light
//    if (lightinfo.type == 0)
//    {
//        ViewLightDir = normalize(mul(float4(lightinfo.direction.xyz, 0.f), view));
    
//        // View 스페이스 상에서 표면의 빛의 세기를 구함
//        fDiffPow = saturate(dot(-ViewLightDir, _vViewNormal));
    
//        // 반사광 세기를 구함
//        // 표면의 빛의 반사벡터       
//        float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _vViewNormal) * _vViewNormal);
        
//        // 시점에서 표면을 향하는 벡터
//        float3 vEye = normalize(_vViewPos);
        
//        // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
//        fSpecPow = saturate(dot(-vEye, vViewReflect));
//        fSpecPow = pow(fSpecPow, 20);
//    }
    
//    // Point Light
//    else if (lightinfo.type == 1)
//    {
//        // ViewSpace 상에서 광원의 위치를 알아낸다.
//        float3 vLightViewPos = mul(float4(lightinfo.position.xyz, 1.f), view);
        
//        // 광원의 위치에서 표면을 향하는 벡터를 구한다.
//        ViewLightDir = _vViewPos - vLightViewPos;
        
//        // 광원에서 표면까지의 거리를 구한다.
//        float fDist = length(ViewLightDir);
        
//        // 광원에서 표면을 향하는 단위벡터를 구한다.        
//        ViewLightDir = normalize(ViewLightDir);
        
//        // 반경대비 거리에 따른 빛의 세기 비율
//        //float fRatio = saturate(1.f - (fDist / lightinfo.fRadius));        
//        float fRatio = cos(saturate(fDist / lightinfo.radius) * 3.1415926535f * 0.5f);
                        
//        // View 스페이스 상에서 표면의 빛의 세기를 구함        
//        fDiffPow = saturate(dot(-ViewLightDir, _vViewNormal)) * fRatio;

//        // 반사광 세기를 구함
//        // 표면의 빛의 반사벡터       
//        float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _vViewNormal) * _vViewNormal);
        
//        // 시점에서 표면을 향하는 벡터
//        float3 vEye = normalize(_vViewPos);
        
//        // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
//        fSpecPow = saturate(dot(-vEye, vViewReflect));
//        fSpecPow = pow(fSpecPow, 20);
//    }
    
//    // SpotLight
//    else
//    {
        
//    }
    
//    // 최종 난반사광
//    _lightcolor.diffuse += lightinfo.color.diffuse * fDiffPow;
    
//    // 최종 반사광 
//    _lightcolor.specular += lightinfo.color.specular * fSpecPow;

//    // 최종 환경광
//    _lightcolor.ambient += lightinfo.color.ambient;
//}

#endif//HLSL
#endif
