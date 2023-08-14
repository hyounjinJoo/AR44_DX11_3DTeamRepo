#ifndef SH_CONST_BUFFER
#define SH_CONST_BUFFER

#include "SH_CommonStruct.hlsli"

//CBUFFER(버퍼이름, 사용할 구조체이름, 버퍼타입, 버퍼번호)
CBUFFER(CB_Global, tCB_Global, b, 0);
CBUFFER(CB_Transform, tCB_Transform, b, 1);
CBUFFER(CB_MaterialData, tCB_MaterialData, b, 2);
CBUFFER(CB_Grid, tCB_Grid, b, 3);
CBUFFER(CB_Animation2D, tCB_Animation2D, b, 4);
CBUFFER(CB_NumberOfLight, tCB_NumberOfLight, b, 5);
CBUFFER(CB_ParticleSystem, tCB_ParticleSystem, b, 6);
CBUFFER(CB_Noise, tCB_Noise, b, 7);
CBUFFER(CB_SBufferCount, tCB_SBufferCount, b, 8);

#ifdef __cplusplus

namespace mh::define
{
	enum class eCBType
	{
		Global = Register_b_CB_Global,	
		Transform = Register_b_CB_Transform,	
		Material = Register_b_CB_MaterialData,	
		Grid = Register_b_CB_Grid,		
		Animation2D = Register_b_CB_Animation2D,	
		numberOfLight = Register_b_CB_NumberOfLight,
		ParticleSystem = Register_b_CB_ParticleSystem,
		Noise = Register_b_CB_Noise,	
		SBufferCount = Register_b_CB_SBufferCount,
		END
	};
};

#endif


#endif
