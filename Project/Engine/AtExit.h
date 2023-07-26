#pragma once

#include <vector>
#include <functional>

class AtExit
{
private:
	static std::vector<std::function<void()>> m_vecAtExit;
public:
	static void AddFunc(std::function<void()> _Func) { m_vecAtExit.push_back(_Func); }
	static void CallAtExit();

private:
	AtExit() = delete;
	~AtExit() = delete;
};
