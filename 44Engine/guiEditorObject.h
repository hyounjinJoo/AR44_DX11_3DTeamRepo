#pragma once
#include "..\Engine_SOURCE\yaGameObject.h"

namespace gui
{
	class EditorObject : public ya::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
		
	};
}