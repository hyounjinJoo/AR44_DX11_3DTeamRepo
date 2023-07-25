#pragma once
#include "IComponent.h"

namespace mh
{
	class AudioListener : public IComponent
	{
	public:
		AudioListener();
		~AudioListener();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	private:
	};
}
