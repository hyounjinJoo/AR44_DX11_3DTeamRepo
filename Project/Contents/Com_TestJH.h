#pragma once

#include <Engine/IComponent.h>

namespace mh
{
	class Com_TestJH :
		public IComponent
	{
	public:
		Com_TestJH();
		virtual ~Com_TestJH();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}


