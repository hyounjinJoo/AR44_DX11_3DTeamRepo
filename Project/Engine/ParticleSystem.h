#pragma once
#include "BaseRenderer.h"
#include "ParticleShader.h"

namespace mh
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};

	class StructedBuffer;

	class ParticleSystem : public BaseRenderer
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
#pragma region Buffer 선언
		GPU::StructedBuffer* mBuffer;
		GPU::StructedBuffer* mSharedBuffer;
#pragma endregion
#pragma region tParticle Shader 관련 변수 선언
		std::shared_ptr<GPU::ParticleShader> mCS;
		renderer::ParticleSystemCB mCBData;
#pragma endregion
#pragma region tParticle System 관련 변수 선언
		Vector4 mStartSize;
		Vector4 mStartColor;
		
		eSimulationSpace mSimulationSpace;
		UINT mMaxParticles;
		float mStartLifeTime;
		float mFrequency;
		float mRadius;

		float mStartSpeed;
		float mTime;
		
		//누적시간
		float mElapsedTime;
#pragma endregion
	};

}
