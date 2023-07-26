#include "EnginePCH.h"

#include "Com_AudioSource.h"
#include "AudioClip.h"
#include "Com_Transform.h"
#include "GameObject.h"

namespace mh
{
	Com_AudioSource::Com_AudioSource()
		: IComponent(define::eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	Com_AudioSource::~Com_AudioSource()
	{

	}

	void Com_AudioSource::Initialize()
	{
	}

	void Com_AudioSource::Update()
	{
	}

	void Com_AudioSource::FixedUpdate()
	{
		Com_Transform& tr = GetOwner()->GetTransform();
		math::Vector3 pos = tr.GetPosition();
		math::Vector3 foward = tr.Foward();

		mAudioClip->Set3DAttributes(pos, foward);
	}

	void Com_AudioSource::Render()
	{
	}

	void Com_AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void Com_AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void Com_AudioSource::SetLoop(bool _loop)
	{
		mAudioClip->SetLoop(_loop);
	}
}
