#include "EnginePCH.h"

#include "Animator.h"

namespace mh
{
	Animator::Animator()
		: IComponent(define::eComponentType::Animator)
		, mAnimations{}
		, mEvents{}
		, mActiveAnimation(nullptr)
		, mbLoop(false)
	{

	}
	Animator::~Animator()
	{
		for (auto anim : mAnimations)
		{
			delete anim.second;
			anim.second = nullptr;
		}

		for (auto evt : mEvents)
		{
			delete evt.second;
			evt.second = nullptr;
		}
	}
	void Animator::Initialize()
	{
	}
	void Animator::Update()
	{
		if (mActiveAnimation == nullptr)
			return;

		tEvents* events = FindEvents(mActiveAnimation->GetAnimationName());
		if (mActiveAnimation->IsComplete())
		{
			if (events)
				events->CompleteEvent();

			if (mbLoop)
				mActiveAnimation->Reset();
		}
		
		if (events) 
		{
			UINT spriteIndex = mActiveAnimation->Update();
			if (spriteIndex != -1
				&& spriteIndex < events->Events.size()
				&& events->Events[spriteIndex].Event)
			{
				events->Events[spriteIndex].Event();
			}
		}

	}
	void Animator::FixedUpdate()
	{
	}
	void Animator::Render()
	{
	}

	bool Animator::Create(const std::wstring& _name, std::shared_ptr<GPU::Texture> _atlas
		, math::Vector2 _leftTop, math::Vector2 _size, math::Vector2 _offset
		, UINT _spriteLegth, float _duration)
	{
		if (_atlas == nullptr)
			return false;

		Animation* animation = FindAnimation(_name);
		if (animation != nullptr)
			return false;

		animation = new Animation();
		animation->Create(_name, _atlas, _leftTop, _size, _offset, _spriteLegth, _duration);

		mAnimations.insert(std::make_pair(_name, animation));

		tEvents* events = new tEvents();
		events->Events.resize(_spriteLegth);
		mEvents.insert(std::make_pair(_name, events));

		return true;
	}

	Animation* Animator::FindAnimation(const std::wstring& _name) const
	{
		auto iter
			= mAnimations.find(_name);

		if (iter == mAnimations.end())
		{
			return nullptr;
		}

		return iter->second;
	}
	
	Animator::tEvents* Animator::FindEvents(const std::wstring& _name) const
	{
		std::map<std::wstring, tEvents*>::const_iterator iter
			= mEvents.find(_name);

		if (iter == mEvents.end())
		{
			return nullptr;
		}

		return iter->second;
	}
	void Animator::Play(const std::wstring& _name, bool _loop)
	{
		Animation* prevAnimation = mActiveAnimation;
		tEvents* events = nullptr;
		if (prevAnimation)
			events = FindEvents(prevAnimation->GetAnimationName());

		if (events)
			events->EndEvent();

		mActiveAnimation = FindAnimation(_name);
		mActiveAnimation->Reset();
		mbLoop = _loop;

		events = FindEvents(mActiveAnimation->GetAnimationName());

		if (events)
			events->StartEvent();
	}

	void Animator::Binds()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->BindShader();
	}

	void Animator::Clear()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Clear();
	}



	std::function<void()>& Animator::GetStartEvent(const std::wstring& _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->StartEvent.Event;
	}
	std::function<void()>& Animator::GetCompleteEvent(const std::wstring& _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->CompleteEvent.Event;
	}
	std::function<void()>& Animator::GetEndEvent(const std::wstring& _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->EndEvent.Event;
	}
	std::function<void()>& Animator::GetEvent(const std::wstring& _name, UINT _index) const
	{
		tEvents* events = FindEvents(_name);

		return events->Events[_index].Event;
	}
}
