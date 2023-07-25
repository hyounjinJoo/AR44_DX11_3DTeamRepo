#include "EnginePCH.h"

#include "AudioSource.h"
#include "AudioClip.h"
#include "Transform.h"
#include "GameObject.h"

namespace mh
{
	AudioSource::AudioSource()
		: IComponent(define::eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	AudioSource::~AudioSource()
	{

	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::FixedUpdate()
	{
		Transform& tr = GetOwner()->GetTransform();
		math::Vector3 pos = tr.GetPosition();
		math::Vector3 foward = tr.Foward();

		mAudioClip->Set3DAttributes(pos, foward);
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void AudioSource::SetLoop(bool _loop)
	{
		mAudioClip->SetLoop(_loop);
	}
}