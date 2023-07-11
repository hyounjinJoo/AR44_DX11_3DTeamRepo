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
    
	//Specular Light�� ����ؾ� �ϹǷ� WorldPos, WorldNormal -> ViewPos, ViewNormal�� ������
	float3 ViewPos : POSITION;
	float3 ViewNormal : NORMAL;
	float intensity : FOG;
};

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

VSOut main(VSIn In)
{
	VSOut OUT = (VSOut) 0.0f;
    
	float4 worldPosition = mul(In.Position, world);
	float4 viewPosition = mul(worldPosition, view);
	float4 ProjPosition = mul(viewPosition, projection);
    
	OUT.Position = ProjPosition;
	OUT.UV = In.UV;

	//View Space �������� ǥ�� Normal Vector�� ����Ѵ�.
	float3 vViewNormal = normalize(mul(float4(In.Normal.xyz, 0.0f), world).xyz);
	vViewNormal = normalize(mul(float4(vViewNormal, 0.0f), view).xyz);
    
	OUT.ViewPos = viewPosition.xyz;
	OUT.ViewNormal = vViewNormal.xyz;
    
	return OUT;
}