#include "PCH_Engine.h"

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
		const auto& vecCom = GetComponents();
		for (size_t i = 0; i < vecCom.size(); ++i)
		{
			if (nullptr == vecCom[i])
				continue;

			vecCom[i]->FixedUpdate();
		}
	}
}
