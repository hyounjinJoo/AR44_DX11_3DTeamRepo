#pragma once
#include "yaGameObject.h"


namespace gui
{
	class DebugObject : public ya::GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		virtual void FixedUpdate() override;
	
	private:
	};
}
