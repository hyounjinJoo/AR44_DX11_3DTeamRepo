#pragma once
#include "IScript.h"
#include "Com_Camera.h"

namespace mh
{
	class GridScript : public IScript
	{
	public:
		GridScript();
		virtual ~GridScript();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		void SetCamera(Com_Camera* camera) { mCamera = camera; }

	private:
		Com_Camera* mCamera;
	};
}
