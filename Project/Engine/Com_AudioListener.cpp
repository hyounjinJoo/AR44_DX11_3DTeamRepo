#include "EnginePCH.h"

#include "Com_AudioListener.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Fmod.h"

namespace mh
{
	Com_AudioListener::Com_AudioListener()
		: IComponent(define::eComponentType::AudioListener)
	{

	}

	Com_AudioListener::~Com_AudioListener()
	{

	}

	void Com_AudioListener::Initialize()
	{

	}

	void Com_AudioListener::Update()
	{

	}

	void Com_AudioListener::FixedUpdate()
	{
		Com_Transform& tr = GetOwner()->GetTransform();
		math::Vector3 pos = tr.GetPosition();
		math::Vector3 foward = tr.Foward();
		math::Vector3 up = tr.Up();

		math::Vector3 vel = { 0.0f, 0.0f, 0.0f };
		Fmod::Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}

	void Com_AudioListener::Render()
	{

	}
}
