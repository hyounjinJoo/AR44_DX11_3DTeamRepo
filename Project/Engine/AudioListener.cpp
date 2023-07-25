#include "EnginePCH.h"

#include "AudioListener.h"
#include "Transform.h"
#include "GameObject.h"
#include "Fmod.h"

namespace mh
{
	AudioListener::AudioListener()
		: IComponent(define::eComponentType::AudioListener)
	{

	}

	AudioListener::~AudioListener()
	{

	}

	void AudioListener::Initialize()
	{

	}

	void AudioListener::Update()
	{

	}

	void AudioListener::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		math::Vector3 pos = tr->GetPosition();
		math::Vector3 foward = tr->Foward();
		math::Vector3 up = tr->Up();

		math::Vector3 vel = { 0.0f, 0.0f, 0.0f };
		Fmod::Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}

	void AudioListener::Render()
	{

	}
}