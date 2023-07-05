
struct LightColor
{
	float4 diffuse;
	float4 specular;
	float4 ambient;
};

struct LightAttribute
{
	LightColor color;
	float4 position;
	float4 direction;
    
	int type;
	float radius;
	float angle;
	int padding;
};

StructuredBuffer<LightAttribute> lightAttributes : register(t13);
StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

//2D
void CalculateLight(in out LightColor pLightColor, float3 position, int idx)
{
	if (0 == lightAttributes[idx].type)
	{
		pLightColor.diffuse += lightAttributes[idx].color.diffuse;
	}
	else if (1 == lightAttributes[idx].type)
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



