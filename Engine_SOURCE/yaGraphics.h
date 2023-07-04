#pragma once
#include <wrl.h>
#include "yaMath.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#define CBSLOT_TRANSFORM		0
#define CBSLOT_MATERIAL			1
#define CBSLOT_GRID				2
#define CBSLOT_ANIMATION		3
#define CBSLOT_NUMBEROFLIGHT	4
#define CBSLOT_PARTICLESYSTEM	5
#define CBSLOT_NOISE			6

using namespace ya::math;
namespace ya::graphics
{
	enum class ValidationMode
	{
		Disabled,
		Enabled,
		GPU,
	};

	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		ALL,
		Count,
	};

	enum class eSamplerType
	{
		Point,
		Linear,
		Anisotropic,
		End,
	};

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		End,
	};

	enum class eDSType
	{
		Less,
		Greater,
		NoWrite,
		None,
		End,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		End,
	};

	enum class eRenderingMode
	{
		Opaque,
		CutOut,
		Transparent,
		PostProcess,
		End,
	};


	struct GpuBuffer
	{
		enum class eType
		{
			Buffer,
			Texture,
			UnknownType,
		} type = eType::UnknownType;

		D3D11_BUFFER_DESC desc;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;
	};

	enum class eCBType
	{
		Transform,
		Material,
		Grid,
		Animation,
		Light,
		ParticleSystem,
		Noise,
		End,
	};

	enum class eGPUParam
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Matrix,
	};

	enum class eSRVType
	{
		SRV,
		UAV,
		End,
	};

	enum class eTextureSlot
	{
		T0,
		T1,
		T2,
		T3,
		T4,
		T5,
		T6,
		T7,

		CubeT8,
		CubeT9,

		Array2DT10,
		Array2DT11,

		End,
	};

	struct DebugMesh
	{
		enums::eColliderType type;
		math::Vector3 position;
		math::Vector3 rotatation;
		math::Vector3 scale;
		
		float radius;
		float duration;
		float time;
	};

	struct LightAttribute
	{
		Vector4 diffuse;
		Vector4 specular;
		Vector4 ambient;
		Vector4 position;
		Vector4 direction;

		enums::eLightType type;
		float radius;
		float angle;
		int padding;
	};

	struct Particle
	{
		Vector4 position;
		Vector4 direction;

		float lifeTime;
		float time;
		float speed;
		UINT active;
	};

	struct ParticleShared
	{
		UINT activeCount;
	};
}