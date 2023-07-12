#include "guiDebugObject.h"


namespace gui
{
	DebugObject::DebugObject()
	{

	}
	DebugObject::~DebugObject()
	{

	}

	void DebugObject::FixedUpdate()
	{
		for (ya::Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->FixedUpdate();
		}
	}
}