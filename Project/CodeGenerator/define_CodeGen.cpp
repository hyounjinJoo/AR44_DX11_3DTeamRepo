#include "pch.h"
#include "define_CodeGen.h"

#include <Engine/define_Util.h>

std::regex define_Preset::Regex::g_VarForbiddenChars::A;
std::wregex define_Preset::Regex::g_VarForbiddenChars::W;

void define_Preset::Regex::g_VarForbiddenChars::CreateVarForbiddenRegex()
{
	std::string strBase = "[";

	using namespace define_Preset::Regex;
	constexpr size_t size = sizeof(arrCharsVarForbiddenA) / sizeof(char*);
	for (size_t i = 0; i < size; ++i)
	{
		strBase += arrCharsVarForbiddenA[i];
	}

	strBase += ']';

	g_VarForbiddenChars::A = std::regex(strBase);
	g_VarForbiddenChars::W = std::wregex(mh::StringConv::ConvertUTF8ToUnicode(strBase));
}

