#pragma once
#include <Engine/IRenderer.h>

namespace mh
{
	class Com_MyRenderer
		: public IRenderer
	{
	public:
		Com_MyRenderer();

		Com_MyRenderer(const Com_MyRenderer& _other) = default;
		CLONE(Com_MyRenderer);

		virtual ~Com_MyRenderer();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
	};
}


