

struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 Normal : NORMAL;
	float3 BiNormal : BINORMAL;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;

	float3 ViewPos : POSITION;
	float3 ViewNormal : NORMAL;

	float3 ViewTanget : TANGENT;
	float3 ViewBiNormal : BINORMAL;
};

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

VSOut main(VSIn In)
{
	VSOut OUT = (VSOut) 0.0f;

	OUT.UV = In.UV;

	return OUT;
}
