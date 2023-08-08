#pragma once

#include <Engine/IComponent.h>

namespace mh
{
	class Com_TestSB :
		public IComponent
	{
	public:
		Com_TestSB();
		virtual ~Com_TestSB();

		virtual void Init() override {}
		virtual void Update() override {}
		virtual void FixedUpdate() override {}
		virtual void Render() override {}

	private:
		int i;

	};
}


