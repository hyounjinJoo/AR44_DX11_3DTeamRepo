#pragma once
#include "Com_Renderer_Mesh.h"

namespace mh
{
	class Com_Animator3D;
	class Com_Renderer_3DAnimMesh :
		public Com_Renderer_Mesh
	{
	public:
		Com_Renderer_3DAnimMesh();
		virtual ~Com_Renderer_3DAnimMesh();

		virtual void Init() override;
		virtual void FixedUpdate() override {};
		virtual void Render() override;

		void SetAnimator(Com_Animator3D* _animator) { mAnimator = _animator; }

	private:
		Com_Animator3D* mAnimator;
	};
}


