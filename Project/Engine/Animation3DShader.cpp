#include "PCH_Engine.h"
#include "Animation3DShader.h"

#include "StructBuffer.h"
#include "DefaultShader/SH_Resource.hlsli"

namespace mh
{
	Animation3DShader::Animation3DShader()
		: ComputeShader(uint3(256u, 1u, 1u))
		, m_pFrameDataBuffer()
		, m_pOffsetMatBuffer()
		, m_pOutputBuffer()
	{ 
	}
	Animation3DShader::~Animation3DShader()
	{
	}


	bool Animation3DShader::Binds()
	{
		if (false == (m_pFrameDataBuffer || m_pOffsetMatBuffer || m_pOutputBuffer))
			return false;

		// 구조화버퍼 전달
		m_pFrameDataBuffer->BindDataSRV(Register_t_g_arrFrameTrans, eShaderStageFlag::CS); // t16
		m_pOffsetMatBuffer->BindDataSRV(Register_t_g_arrOffset, eShaderStageFlag::CS); // t17
		m_pOutputBuffer->BindDataUAV(Register_u_g_arrFinalMat);   // u0

		uint3 DataCounts{};
		DataCounts.x = mAnim3DData.BoneCount;
		DataCounts.y = 1;
		DataCounts.z = 1;
		
		CalculateGroupCount(DataCounts);

		return true;
	}


	void Animation3DShader::Clear()
	{
		// 전달한 구조화버퍼 클리어	
		if (nullptr != m_pFrameDataBuffer)
		{
			m_pFrameDataBuffer->UnBind();
			m_pFrameDataBuffer = nullptr;
		}

		if (nullptr != m_pOffsetMatBuffer)
		{
			m_pOffsetMatBuffer->UnBind();
			m_pOffsetMatBuffer = nullptr;
		}

		if (nullptr != m_pOutputBuffer)
		{
			m_pOutputBuffer->UnBind();
			m_pOutputBuffer = nullptr;
		}
	}
}
