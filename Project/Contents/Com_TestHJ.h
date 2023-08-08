#pragma once

#include <Engine/IComponent.h>

namespace mh
{
	class Com_TestHJ :
		public IComponent
	{
	public:
		Com_TestHJ();
		virtual ~Com_TestHJ();

		virtual void Init() override {}
		virtual void Update() override {}
		virtual void FixedUpdate() override {}
		virtual void Render() override {}

	private:
		int i;

	};
}


