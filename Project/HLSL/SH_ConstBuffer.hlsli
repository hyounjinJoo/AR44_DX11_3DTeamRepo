#ifndef SH_CONST_BUFFER
#define SH_CONST_BUFFER

#include "SH_CommonStruct.hlsli"


//////////////////////////////////
//새로운 상수버퍼 생성시 해줘야 할것
//구조체 선언
//CBUFFER(버퍼이름, 사용할 구조체이름, 버퍼타입, 버퍼번호) 작성
//아래 C++ enum class에 해당 번호 추가
//RenderMgr에서 상수버퍼 추가
/////////////////////////////////


//CBUFFER(버퍼이름, 사용할 구조체이름, 버퍼타입, 버퍼번호)
CBUFFER(CB_Global, tCB_Global, b, 0);
CBUFFER(CB_Transform, tCB_Transform, b, 1);
CBUFFER(CB_MaterialData, tCB_MaterialData, b, 2);
CBUFFER(CB_ComputeShader, tCB_ComputeShader, b, 3);
CBUFFER(CB_Grid, tCB_Grid, b, 4);
CBUFFER(CB_Animation2D, tCB_Animation2D, b, 5);
CBUFFER(CB_NumberOfLight, tCB_NumberOfLight, b, 6);
CBUFFER(CB_ParticleSystem, tCB_ParticleSystem, b, 7);
CBUFFER(CB_Noise, tCB_Noise, b, 8);
CBUFFER(CB_SBufferCount, tCB_SBufferCount, b, 9);
CBUFFER(CB_Animation3D, tCB_Animation3D, b, 10);
CBUFFER(CB_UniformData, tCB_UniformData, b, 11);

#ifdef __cplusplus

namespace mh::define
{
	enum class eCBType
	{
		Global = Register_b_CB_Global,	
		Transform = Register_b_CB_Transform,	
		Material = Register_b_CB_MaterialData,	
		ComputeShader = Register_b_CB_ComputeShader,	
		Grid = Register_b_CB_Grid,		
		Animation2D = Register_b_CB_Animation2D,	
		numberOfLight = Register_b_CB_NumberOfLight,
		ParticleSystem = Register_b_CB_ParticleSystem,
		Noise = Register_b_CB_Noise,	
		SBufferCount = Register_b_CB_SBufferCount,
		Animation3D = Register_b_CB_Animation3D,
		UniformData = Register_b_CB_UniformData,
		END
	};
};

#endif


#endif
