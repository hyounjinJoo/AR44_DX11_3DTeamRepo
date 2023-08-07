#pragma once

#include <Engine/IComponent.h>

namespace mh
{
	class Com_TestCH :
		public IComponent
	{
	public:
		Com_TestCH();
		virtual ~Com_TestCH();

		virtual void Init() override {}
		virtual void Update() override {}
		virtual void FixedUpdate() override {}
		virtual void Render() override {}

	private:
		int i;

	};
}


