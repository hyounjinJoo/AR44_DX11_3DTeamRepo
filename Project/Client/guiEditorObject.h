#pragma once

#include <Engine/GameObject.h>

namespace gui
{
	class EditorObject : public mh::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
		
	};
}
