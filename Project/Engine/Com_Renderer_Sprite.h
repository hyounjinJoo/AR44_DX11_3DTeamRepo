#pragma once
#include "Com_Renderer_Mesh.h"


using namespace mh;
namespace mh
{
	class Com_Renderer_Sprite : public Com_Renderer_Mesh
	{
	public:
		Com_Renderer_Sprite();

		Com_Renderer_Sprite(const Com_Renderer_Sprite& _other) = default;

		CLONE(Com_Renderer_Sprite);

		virtual ~Com_Renderer_Sprite();
		
		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
