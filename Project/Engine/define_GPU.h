#pragma once

#include <wrl.h>
using Microsoft::WRL::ComPtr;
namespace Microsoft::WRL
{
	template <class T> struct is_ComPtr : std::false_type {};
	template <class T> struct is_ComPtr<Microsoft::WRL::ComPtr<T>> : std::true_type {};
	template <class T> inline constexpr bool is_ComPtr_v = is_ComPtr<T>::value;
}


#include "define_Enum.h"
#include "define_Macro.h"

#include "DefaultShader/SH_ConstBuffer.hlsli"
#include "DefaultShader/SH_Resource.hlsli"


namespace mh::define
{
	constexpr const int MRT_MAX = 8;

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
		END,
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
		END,
	};

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		END,
	};

	enum class eDSType
	{
		Less,
		Greater,
		NoWrite,
		None,
		END,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		END,
	};

	namespace strKey
	{
		constexpr const char* eSamplerType[(int)mh::define::eSamplerType::END]
		{
			"Point",
			"Linear",
			"Anisotropic",
		};

		constexpr const char* eRSType[(int)mh::define::eRSType::END]
		{
			"SolidBack",
			"SolidFront",
			"SolidNone",
			"WireframeNone",
		};

		constexpr const char* eDSType[(int)mh::define::eDSType::END]
		{
			"Less",
			"Greater",
			"NoWrite",
			"None",
		};

		constexpr const char* eBSType[(int)mh::define::eBSType::END]
		{
			"Default",
			"AlphaBlend",
			"OneOne",
		};
	}

	enum class eRenderingMode
	{
		None = -1,

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
		
		END,
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
		END,
	};

	enum class eTextureSlot
	{
		Tex_0,
		Tex_1,
		Tex_2,
		Tex_3,
		Tex_4,
		Tex_5,
		Tex_6,
		Tex_7,


		Albedo = Register_t_AlbedoTexture,
		Normal = Register_t_NormalTexture,
		Specular = Register_t_SpecularTexture,
		Emissive = Register_t_EmissiveTexture,
		RoughnessAndMetalic = Register_t_RoughnessMetalicTexture,

		AlbedoTarget = Register_t_AlbedoTarget,
		NormalTarget = Register_t_NormalTarget,
		SpecularTarget = Register_t_SpecularTarget,
		EmissiveTarget = Register_t_EmissiveTarget,
		RoughnessAndMetalicTarget = Register_t_RoughnessMetalicTarget,
		PositionTarget = Register_t_PositionTarget,

		DiffuseLightTarget = Register_t_DiffuseLightTarget,
		SpecularLightTarget = Register_t_SpecularLightTarget,

		//SkyBoxCube = Register_t_skyBoxCube,
		//CubeT8,
		//CubeT9,

		//Array2DT10,
		//Array2DT11,

		//END = 16
		END = 8,
	};

	enum class eMRT_Defferd
	{
		AlbedoTarget,
		NormalTarget,
		SpecularTarget,
		EmissiveTarget,
		PositionTarget,
		END
	};

	enum class eMRT_Light
	{
		DiffuseLightTarget,
		SpecularLightTarget,
		END
	};

	enum class eBufferViewType
	{
		NONE,
		SRV,
		UAV,
		RTV,
		DSV,
	};




}
