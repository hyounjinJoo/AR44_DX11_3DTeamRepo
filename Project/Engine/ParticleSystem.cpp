
#include "EnginePCH.h"

#include "ParticleSystem.h"
#include "Mesh.h"
#include "GameResources.h"
#include "Material.h"
#include "StructedBuffer.h"
#include "Transform.h"
#include "GameObject.h"
#include "Texture.h"
#include "TimeManager.h"

namespace mh
{
	using namespace mh::GPU;

	ParticleSystem::ParticleSystem()
		: BaseRenderer(eComponentType::ParticleSystem)
		, mMaxParticles(100)
		, mStartSize(Vector4(50.0f, 50.0f, 1.0f, 1.0f))
		, mStartColor(Vector4(1.0f, 0.2f, 0.2f, 1.0f))
		, mStartLifeTime(3.0f)
		, mFrequency(1.0f)
		, mTime(0.0f)
		, mCBData{}
		, mSimulationSpace(eSimulationSpace::World)
		, mRadius(500.0f)
		, mStartSpeed(200.0f)
		, mElapsedTime(0.0f)

	{

	}

	ParticleSystem::~ParticleSystem()
	{
		delete mBuffer;
		mBuffer = nullptr;

		delete mSharedBuffer;
		mSharedBuffer = nullptr;
	}

	void ParticleSystem::Initialize()
	{
		mCS = GameResources::Find<ParticleShader>("ParticleCS");

		std::shared_ptr<Mesh> point = GameResources::Find<Mesh>("PointMesh");
		SetMesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = GameResources::Find<Material>("ParticleMaterial");
		SetMaterial(material);

		std::shared_ptr<Texture> tex = GameResources::Find<Texture>("CartoonSmoke");
		material->SetTexture(eTextureSlot::Albedo, tex);

		tParticle particles[100] = {};
		Vector4 startPos = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mMaxParticles; i++)
		{
			particles[i].position = Vector4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].active = 0;
			particles[i].direction =
				Vector4(cosf( (float)i * (XM_2PI / (float)mMaxParticles) )
					, sin( (float)i * (XM_2PI / (float)mMaxParticles)), 0.0f, 1.0f );

			particles[i].speed = 100.0f;
		}

		mBuffer = new GPU::StructedBuffer();
		mBuffer->Create(sizeof(tParticle), mMaxParticles, eSRVType::UAV, particles);

		mSharedBuffer = new GPU::StructedBuffer();
		mSharedBuffer->Create(sizeof(tParticleShared), 1, eSRVType::UAV, nullptr, true);
	}

	void ParticleSystem::Update()
	{
	}

	void ParticleSystem::FixedUpdate()
	{
		//파티클 생성 시간
		float aliveTime = 1.0f / mFrequency;

		//누적시간
		mTime += TimeManager::DeltaTime();
		if (aliveTime < mTime)
		{
			float f = (mTime / aliveTime);
			UINT iAliveCount = (UINT)f;
			mTime = f - std::floor(f);

			tParticleShared shared = { 5, };
			mSharedBuffer->SetData(&shared, 1);
		}
		else
		{
			tParticleShared shared = {  };
			mSharedBuffer->SetData(&shared, 1);
		}

		mMaxParticles = mBuffer->GetStride();
		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		mCBData.WorldPosition = Vector4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.MaxParticles = mMaxParticles;
		mCBData.Radius = mRadius;
		mCBData.SimulationSpace = (UINT)mSimulationSpace;
		mCBData.StartSpeed = mStartSpeed;
		mCBData.StartSize = mStartSize;
		mCBData.StartColor = mStartColor;
		mCBData.StartLifeTime = mStartLifeTime;
		mCBData.DeltaTime = TimeManager::DeltaTime();
		mCBData.ElapsedTime += TimeManager::DeltaTime();

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::ParticleSystem];
		cb->SetData(&mCBData);
		cb->Bind(eShaderStage::ALL);

		mCS->SetSharedStrutedBuffer(mSharedBuffer);
		mCS->SetStrcutedBuffer(mBuffer);
		mCS->OnExcute();
	}

	void ParticleSystem::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		mBuffer->BindSRV(eShaderStage::GS, 15);

		GetMaterial()->Bind();
		GetMesh()->RenderInstanced(mMaxParticles);

		mBuffer->Clear();
	}
}
