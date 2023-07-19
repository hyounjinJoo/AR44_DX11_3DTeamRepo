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
		for (mh::IComponent* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->FixedUpdate();
		}
	}
}