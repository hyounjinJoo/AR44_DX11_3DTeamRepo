#pragma once

#ifndef JSON_AMALGAMATED_H_INCLUDED
#include "json.h"
#endif
#include "../define_Util.h"
#include "../define_Enum.h"
#include <wrl.h>

namespace Microsoft::WRL
{
	template <class T> struct is_ComPtr : std::false_type {};
	template <class T> struct is_ComPtr<Microsoft::WRL::ComPtr<T>> : std::true_type {};
	template <class T> inline constexpr bool is_ComPtr_v = is_ComPtr<T>::value;
}

namespace Json
{
	//template <typename T, typename std::enable_if_t<(std::is_smart_ptr_v<T> || std::is_pointer_v<T>), bool>* = nullptr>
	//std::string MHConvertWrite(const T& _srcT)
	//{
	//	std::string strReturn = "";
	//	if (_srcT)
	//	{
	//		strReturn = _srcT->GetKey();
	//		if (strReturn.empty())
	//		{
	//			ERROR_MESSAGE_W(L"String Key가 존재하지 않아서 저장 불가.");
	//		}
	//	}
	//	return strReturn;
	//}

	template <typename T, typename std::enable_if_t<(std::is_same_v<std::string, T> || std::is_integral_v<T>), bool>* = nullptr>
	T MHConvertWrite(const T& _srcT)
	{
		return _srcT;
	}

	template <typename T, typename std::enable_if_t<(std::is_enum_v<T>), bool>* = nullptr>
	int MHConvertWrite(const T& _srcT)
	{
		return (int)_srcT;
	}

	template <typename T, typename 
		std::enable_if_t < (
		!std::is_smart_ptr_v<T> && 
		!std::is_pointer_v<T> && 
		!std::is_same_v<std::string, T> &&
		!std::is_integral_v<T> && 
		!std::is_enum_v<T>)
		, bool>* = nullptr>
	std::string MHConvertWrite(const T& _srcT)
	{
		//컨테이너는 변환 불가
		static_assert(false == std::is_container_v<T>);
		return mh::StringConv::Convert_T_to_String(_srcT);
	}


	//template <typename T, typename std::enable_if_t<(std::is_smart_ptr_v<T> || std::is_pointer_v<T>), bool>* = nullptr>
	//std::string MHConvertRead(const Json::Value& _jVal)
	//{
	//	return _jVal.asString();
	//}

	template <typename T, typename std::enable_if_t<(std::is_same_v<std::string, T> || std::is_integral_v<T>), bool>* = nullptr>
	T MHConvertRead(const Json::Value& _jVal)
	{
		return _jVal.as<T>();
	}

	template <typename T, typename std::enable_if_t<(std::is_enum_v<T>), bool>* = nullptr>
	T MHConvertRead(const Json::Value& _jVal)
	{
		return (T)_jVal.asInt();
	}


	template <typename T, typename
		std::enable_if_t < (
			!std::is_smart_ptr_v<T> &&
			!std::is_pointer_v<T> &&
			!std::is_same_v<std::string, T> &&
			!std::is_integral_v<T> &&
			!std::is_enum_v<T>)
		, bool>* = nullptr>
	T MHConvertRead(const Json::Value& _jVal)
	{
		//컨테이너는 변환 불가
		static_assert(false == std::is_container_v<T>);
		return mh::StringConv::Convert_String_to_T<T>(_jVal.asString());
	}

	template <typename T>
	void MHSaveValue(Json::Value* _pJson, const char* _strKey, const T& _srcT)
	{
		Json::Value& jVal = (*_pJson);
		jVal[_strKey] = MHConvertWrite<T>(_srcT);
	}

	template <typename T>
	bool MHLoadValue(const Json::Value* _pJson, const char* _strKey, T& _destT)
	{
		bool bResult = false;
		if (_pJson->isMember(_strKey))
		{
			const Json::Value& jVal = (*_pJson)[_strKey];
			if constexpr (std::is_same_v<std::string, T>)
			{
				_destT = jVal.asString();
				bResult = true;
			}
			else if constexpr (std::is_integral_v<T>)
			{
				_destT = jVal.as<T>();
				bResult = true;
			}
			else if constexpr (std::is_enum_v<T>)
			{
				_destT = (T)jVal.asInt();
				bResult = true;
			}
			else
			{
				_destT = MHConvertRead<T>(jVal.asString());
				bResult = true;
			}
		}


		return bResult;
	}



	template <typename T, typename std::enable_if_t<(
		true == std::is_vector_v<T>
		), bool>* = nullptr>
	static T MHGetJsonArray(const Json::Value* _pJson, const char* _strKey)
	{
		using valType = T::value_type;

		//포인터에 대한 값은 추가할 수 없음.
		static_assert(false == std::is_pointer_v<valType> || false == std::is_smart_ptr_v<valType>);

		T vecT{};
		if (_pJson->isMember(_strKey))
		{
			const Json::Value& jVal = *_pJson;
			if (jVal.isArray())
			{
				for (Json::ValueConstIterator iter = jVal.begin(); iter != jVal.end(); ++iter)
				{
					vecT.push_back(MHConvertRead<valType>(*iter));
				}
			}
		}
		return vecT;
	}


	static std::vector<std::string> MHGetJsonArrayPtr(const Json::Value* _pJson, const char* _strKey)
	{
		std::vector<std::string> retVec{};
		if (_pJson->isMember(_strKey))
		{
			const Json::Value& jVal = *_pJson;
			if (jVal.isArray())
			{
				for (Json::ValueConstIterator iter = jVal.begin(); iter != jVal.end(); ++iter)
				{
					retVec.push_back(iter->asString());
				}
			}
		}
		return retVec;
	}


	//Value가 들어있는 map만
	template <typename T, typename std::enable_if_t<(
		true == std::is_pair_container_v<T>
		), bool>* = nullptr>
	static T MHGetJsonMap(const Json::Value* _pJson, const char* _strKey)
	{
		using keyType = T::key_type;
		using valType = T::mapped_type;
		static_assert(false == std::is_pointer_v<keyType> || false == std::is_smart_ptr_v<keyType>);
		static_assert(false == std::is_pointer_v<valType> || false == std::is_smart_ptr_v<valType>);

		T vecT{};
		if (_pJson->isMember(_strKey))
		{
			const Json::Value& jVal = *_pJson;
			for (Json::ValueConstIterator iter = jVal.begin(); iter != jVal.end(); ++iter)
			{
				keyType key = MHConvertRead<keyType>(iter.key());
				valType val = MHConvertRead<valType>(*iter);
				vecT.insert(std::make_pair(key, val));
			}
		}
		return vecT;
	}
}

#define JSONKEY(_varName) #_varName

#define MH_SAVE_VALUE(_pJson, _var) Json::MHSaveValue(_pJson, #_var, _var)
#define MH_LOAD_VALUE(_pJson, _var) Json::MHLoadValue(_pJson, #_var, _var)

