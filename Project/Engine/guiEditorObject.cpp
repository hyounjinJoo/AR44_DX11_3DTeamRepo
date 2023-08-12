#include "PCH_Engine.h"

#include "guiEditorObject.h"

namespace gui
{
	EditorObject::EditorObject()
		: GameObject()
	{
	}
	EditorObject::~EditorObject()
	{
		
	}
	void EditorObject::Init()
	{
		GameObject::Init();
	}
	void EditorObject::Update()
	{
		GameObject::Update();
	}
	void EditorObject::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void EditorObject::Render()
	{
		GameObject::Render();
	}
}
