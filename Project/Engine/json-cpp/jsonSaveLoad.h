#pragma once

#ifndef JSON_AMALGAMATED_H_INCLUDED
#include "json.h"
#endif
#include "../define_UtilFunc.h"
#include "../define_Enum.h"

namespace Json
{
	template <typename T>
	static void MHSave(Json::Value* _pJson, const char* _strKey, const T& _srcT)
	{
		if constexpr (std::is_same_v<std::string, T> || std::is_integral_v<T>)
		{
			(*_pJson)[_strKey] = _srcT;
		}
		else if constexpr (std::is_enum_v<T>)
		{
			(*_pJson)[_strKey] = (int)_srcT;
		}
		else
		{
			(*_pJson)[_strKey] = mh::StringConv::Convert_T_to_String(_srcT);
		}
	}

	template <typename T>
	static bool MHLoad(const Json::Value* _pJson, const char* _strKey, T& _destT)
	{
		bool bResult = false;

		if constexpr (std::is_same_v<std::string, T> || std::is_integral_v<T>)
		{
			if (_pJson->isMember(_strKey))
			{
				_destT = (*_pJson)[_strKey].as<T>();
				bResult = true;
			}
		}
		else if constexpr (std::is_enum_v<T>)
		{
			if (_pJson->isMember(_strKey))
			{
				_destT = (T)(*_pJson)[_strKey].asInt();
				bResult = true;
			}
		}
		else
		{
			if (_pJson->isMember(_strKey))
			{
				bResult = mh::StringConv::Convert_String_to_T((*_pJson)[_strKey].asString(), _destT);
				bResult = true;
			}
		}

		return bResult;
	}
}

#define MHJSONSAVE(_pJson, _var) Json::MHSave(_pJson, #_var, _var)
#define MHJSONLOAD(_pJson, _var) Json::MHLoad(_pJson, #_var, _var)

