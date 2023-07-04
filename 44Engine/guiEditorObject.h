#pragma once
#include "..\Engine_SOURCE\yaGameObject.h"


namespace gui
{
	class EditorObject : public ya::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		virtual void Initalize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		
	};
}