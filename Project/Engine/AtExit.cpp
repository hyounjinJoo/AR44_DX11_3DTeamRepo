#include "EnginePCH.h"
#include "AtExit.h"

std::vector<std::function<void()>> AtExit::m_vecAtExit;

void AtExit::CallAtExit()
{
	for (int i = (int)m_vecAtExit.size() - 1; i >= 0; --i)
	{
		m_vecAtExit[i]();
	}
}

