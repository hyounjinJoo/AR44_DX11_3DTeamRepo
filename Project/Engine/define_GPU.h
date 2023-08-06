#pragma once
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "SimpleMath.h"

#include "define_Enum.h"
#include "define_Macro.h"

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#define CBSLOT_GLOBAL			0
#define CBSLOT_TRANSFORM		1
#define CBSLOT_MATERIAL			2
#define CBSLOT_GRID				3
#define CBSLOT_ANIMATION		4
#define CBSLOT_NUMBEROFLIGHT	5
#define CBSLOT_PARTICLESYSTEM	6
#define CBSLOT_NOISE			7
#define CBSLOT_SBUFFER			8

namespace mh::define
{
	constexpr const int MRT_MAX = 8;

	using namespace mh::define;
	
	enum class eValidationMode
	{
		Disabled,
		Enabled,
		GPU,
	};

	enum class eGSStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		END
	};

	namespace strKey
	{
		STRKEY ArrGSPrefix[(int)eGSStage::END] =
		{
			"_0VS_",
			"_1HS_",
			"_2DS_",
			"_3GS_",
			"_4PS_",
		};
		STRKEY CSPrefix = "CS_";

		STRKEY Ext_ShaderSetting = ".json";
	}


	enum class eMRTType
	{
		Swapchain,
		Deffered,
		Light,
		Shadow,
		End,
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
		END
	};

	namespace eShaderStageFlag
	{
		enum Flag
		{
			NONE = 0,
			VS = BIT_MASK(0),
			HS = BIT_MASK(1),
			DS = BIT_MASK(2),
			GS = BIT_MASK(3),
			PS = BIT_MASK(4),
			CS = BIT_MASK(5),
			ALL = VS | HS | DS | GS | PS | CS,
		};
	}
	using eShaderStageFlag_ = int;

	namespace SHADER_VERSION
	{
		constexpr const char* GS[(int)eShaderStage::END]
			= {
			"vs_5_0",
			"hs_5_0",
			"ds_5_0",
			"gs_5_0",
			"ps_5_0"
		};
		constexpr const char* CS = "cs_5_0";
	}

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
		//Deffered
		DefferdOpaque,
		DefferdMask,

		//광원처리
		Light,

		//Forward
		Opaque,
		CutOut,
		Transparent,
		PostProcess,
		None,
		End,
	};


	enum class eCBType
	{
		Global,
		Transform,
		Material,
		Grid,
		Animation,
		Light,
		ParticleSystem,
		Noise,
		SBuffer,
		End,
	};


	enum class eGPUParam
	{
		Int,
		Float,
		float2,
		float3,
		float4,
		MATRIX,
	};

	enum class eSRVType
	{
		SRV,
		UAV,
		End,
	};

	enum class eTextureSlot
	{
		Albedo,
		Normal,

		PositionTarget,
		NormalTarget,
		AlbedoTarget,
		SpecularTarget,
		DiffuseLightTarget,
		SpecularLightTarget,

		//CubeT8,
		//CubeT9,

		//Array2DT10,
		//Array2DT11,

		End,
	};

	enum class eMRT_Defferd
	{
		PositionTarget,
		NormalTarget,
		AlbedoTarget,
		SpecularTarget,
		End
	};

	enum class eMRT_Light
	{
		DiffuseLightTarget,
		SpecularLightTarget
	};

	enum class eBufferViewType
	{
		NONE,
		SRV,
		UAV,
		RTV,
		DSV,
	};

	struct tDebugMesh
	{
		define::eColliderType type;
		float3 position;
		float3 rotatation;
		float3 scale;
		
		float radius;
		float duration;
		float time;
	};

	struct tLightAttribute
	{
		float4 diffuse;
		float4 specular;
		float4 ambient;

		float4 position;
		float4 direction;

		define::eLightType type;
		float radius;
		float angle;
		int padding;
	};

	struct tParticle
	{
		float4 position;
		float4 direction;

		float lifeTime;
		float time;
		float speed;
		uint active;
	};

	struct tParticleShared
	{
		uint activeCount;
	};


}
