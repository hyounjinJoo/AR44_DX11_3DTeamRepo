
#include "EnginePCH.h"

#include "Com_Renderer_ParticleSystem.h"
#include "Mesh.h"
#include "ResMgr.h"
#include "Material.h"
#include "StructedBuffer.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Texture.h"
#include "TimeManager.h"

namespace mh
{
	using namespace mh::GPU;

	Com_Renderer_ParticleSystem::Com_Renderer_ParticleSystem()
		: mMaxParticles(100)
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

	Com_Renderer_ParticleSystem::~Com_Renderer_ParticleSystem()
	{
		delete mBuffer;
		mBuffer = nullptr;

		delete mSharedBuffer;
		mSharedBuffer = nullptr;
	}

	void Com_Renderer_ParticleSystem::Initialize()
	{
		using namespace strKey::Default;
		mCS = ResMgr::GetInst()->Find<ParticleShader>(shader::compute::ParticleCS);

		std::shared_ptr<Mesh> point = ResMgr::GetInst()->Find<Mesh>(mesh::PointMesh);
		SetMesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = ResMgr::GetInst()->Find<Material>(material::ParticleMaterial);
		SetMaterial(material);

		std::shared_ptr<Texture> tex = ResMgr::GetInst()->Find<Texture>(texture::CartoonSmoke);
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

	void Com_Renderer_ParticleSystem::Update()
	{
	}

	void Com_Renderer_ParticleSystem::FixedUpdate()
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
		Vector3 pos = GetOwner()->GetTransform().GetPosition();
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

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Com_Renderer_ParticleSystem];
		cb->SetData(&mCBData);
		cb->Bind(eShaderStage::ALL);

		mCS->SetSharedStrutedBuffer(mSharedBuffer);
		mCS->SetStrcutedBuffer(mBuffer);
		mCS->OnExcute();
	}

	void Com_Renderer_ParticleSystem::Render()
	{
		GetOwner()->GetTransform().SetConstantBuffer();
		mBuffer->BindSRV(eShaderStage::GS, 15);

		GetMaterial()->Bind();
		GetMesh()->RenderInstanced(mMaxParticles);

		mBuffer->Clear();
	}
}
