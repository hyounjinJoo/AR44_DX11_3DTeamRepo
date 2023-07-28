#include "EnginePCH.h"

#include "Com_Animator.h"

namespace mh
{
	Com_Animator::Com_Animator()
		: IComponent(define::eComponentType::Animator)
		, mAnimations{}
		, mEvents{}
		, mActiveAnimation(nullptr)
		, mbLoop(false)
	{

	}
	Com_Animator::~Com_Animator()
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
	void Com_Animator::Initialize()
	{
	}
	void Com_Animator::Update()
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
	void Com_Animator::FixedUpdate()
	{
	}
	void Com_Animator::Render()
	{
	}

	bool Com_Animator::Create(const std::string_view _name, std::shared_ptr<Texture> _atlas
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

	Animation* Com_Animator::FindAnimation(const std::string_view _name) const
	{
		const auto iter = mAnimations.find(_name);

		if (iter == mAnimations.end())
		{
			return nullptr;
		}

		return iter->second;
	}
	
	Com_Animator::tEvents* Com_Animator::FindEvents(const std::string_view _name) const
	{
		const auto iter
			= mEvents.find(_name);

		if (iter == mEvents.end())
		{
			return nullptr;
		}

		return iter->second;
	}
	void Com_Animator::Play(const std::string_view _name, bool _loop)
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

	void Com_Animator::Binds()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->BindShader();
	}

	void Com_Animator::Clear()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Clear();
	}



	std::function<void()>& Com_Animator::GetStartEvent(const std::string_view _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->StartEvent.Event;
	}
	std::function<void()>& Com_Animator::GetCompleteEvent(const std::string_view _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->CompleteEvent.Event;
	}
	std::function<void()>& Com_Animator::GetEndEvent(const std::string_view _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->EndEvent.Event;
	}
	std::function<void()>& Com_Animator::GetEvent(const std::string_view _name, UINT _index) const
	{
		tEvents* events = FindEvents(_name);

		return events->Events[_index].Event;
	}
}
