#include "PCH_Engine.h"
#include "AtExit.h"

std::stack<std::function<void()>> AtExit::mAtExitFuncs;

void AtExit::CallAtExit()
{
	while (false == mAtExitFuncs.empty())
	{
		mAtExitFuncs.top()();
		mAtExitFuncs.pop();
	}
}

