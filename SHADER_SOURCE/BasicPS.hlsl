#include "globals.hlsli"

struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
    
	float3 ViewPos : POSITION;
	float3 ViewNormal : NORMAL;
	float intensity : FOG;
};

//diffuse
//specular
//ambient

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

float4 main(VSOut In) : SV_Target
{
	float4 OutColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    //light 구조화 버퍼의 0번째 광원 정보를 가져온다.(이 부분은 나중에 순회를 돌아줘야 함)
	LightAttribute lightAttribute = lightAttributes[0];
    
    
    //(1) 광원의 방향을 월드 좌표계에서 뷰 좌표계(카메라 기준 좌표계)로 변환
	float3 ViewLightDir = normalize(mul(float4(lightAttribute.direction.xyz, 0.0f), view));
    
    //(2) 빛의 강도를 구한다.
    //뷰 공간에서의 빛의 방향을 반대 방향으로 바꾸고(음수로 전환)
    //(뷰 공간 안에서의) 빛의 방향과 정점의 방향을 내적한다. -> 이때 두 벡터는 단위 벡터이므로 코사인값이 구해짐.
    //빛의 강도는 '배율' 이므로 0 ~ 1 사이 float 값으로 제한
	float intensity = saturate(dot(-ViewLightDir, In.ViewNormal));
	float fSpecPow = 0.0f;
    
    //(3) 뷰 스페이스 상에서 표면의 빛의 세기를 구해준다.
    //반사광의 세기를 구해준다.
	float3 vViewReflect
    = normalize(ViewLightDir + 2.0f * dot(-ViewLightDir, In.ViewNormal) * In.ViewNormal);
    
    //(4) 시점에서 표면을 향하는 벡터를 구한다.
    //지금 상태는 뷰 스페이스 기준(카메라 위치가 0, 0, 0)이므로 
    //카메라 기준으로 모든 위치는 카메라로부터의 방향 벡터와 같다.
    //그러므로, 뷰 공간에서의 좌표를 노멀라이즈 해주면, 카메라로부터 지금 처리중인 픽셀(표면)로까지의 방향 벡터가 된다.
	float3 vEye = normalize(In.ViewPos);
    
    //(5) 시선 벡터랑 반사벡터를 내적해서 반사광의 세기를 구한다.
	fSpecPow = saturate(dot(-vEye, vViewReflect));
	fSpecPow = pow(fSpecPow, 30);
    
    
    //(6) 계산한 수치에 더해 다른 빛의 요소들의 색깔까지 더해서 최종적인 빛 색상을 계산한다.
	OutColor.rgb = (OutColor.rgb * lightAttribute.color.diffuse.rgb * intensity
                    + lightAttribute.color.specular.rgb * fSpecPow * 1.0f
                    + (OutColor.xyz * lightAttribute.color.ambient.rgb));
    
    //float4 Out = float4(1.0f, 0.0f, 1.0f, 1.0f);
    //float3 LightDir = normalize(globalLightDir);
    //float intensity = saturate(dot(-LightDir, In.Normal));
    
    //Out.xyz = (intensity * OutColor.xyz * globalLightColor)
    //            + (OutColor.xyz * globalLightAmb);
    
	return OutColor;
        
}