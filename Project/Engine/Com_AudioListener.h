#pragma once
#include "IComponent.h"

namespace mh
{
	class Com_AudioListener : public IComponent
	{
	public:
		Com_AudioListener();
		virtual ~Com_AudioListener();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	private:
	};
}
