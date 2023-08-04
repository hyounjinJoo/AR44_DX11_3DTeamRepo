#pragma once

#ifndef JSON_AMALGAMATED_H_INCLUDED
#include "json.h"
#endif
#include "../define_Util.h"
#include "../define_Enum.h"
#include <wrl.h>
#include <utility>

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

	template <typename T, typename std::enable_if_t<(
		std::is_same_v<std::string, T> || 
		std::is_integral_v<T>
		), bool>* = nullptr>
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
	void MHSavePtr(Json::Value* _pJson, const char* _strKey, const T& _srcT)
	{
		static_assert(
			true == std::is_pointer_v<T> ||
			true == std::is_smart_ptr_v<T>
			);

		Json::Value& jVal = (*_pJson)[_strKey];
		jVal = "";
		if (_srcT)
		{
			jVal = _srcT->GetKey();
		}

	}


	template <typename T, typename std::enable_if_t<
		(true == std::is_vector_v<T> ||
		true == std::is_std_array_v<T>)
		, int>* = nullptr>
	void MHSaveVector(Json::Value* _pJson, const char* _strKey, const T& _srcT)
	{
		using valType = T::value_type;

		//벡터(또는 배열) + 포인터 아닐것
		static_assert(
			false == std::is_pointer_v<valType> &&
			false == std::is_smart_ptr_v<valType>
				);

		//Value가 들어있는 vector 저장하는법
		//1. json의 타입을 array value로 만들어준다.
		(*_pJson)[_strKey] = Json::Value(Json::arrayValue);

		//2. 만들어진 데이터 컨테이너를 레퍼런스로 받아온다.
		Json::Value& jVal = (*_pJson)[_strKey];
		for (size_t i = 0; i < _srcT.size(); ++i)
		{
			//3. 순회돌면서 하나씩 추가한다.
			jVal.append(MHConvertWrite(_srcT[i]));
		}
	}


	//GetKey()가 있는 클래스 포인터 한정으로 사용가능
	template <typename T, typename std::enable_if_t<
		(true == std::is_vector_v<T> ||
		true == std::is_std_array_v<T>)
		, int>* = nullptr>
	void MHSaveVectorPtr(Json::Value* _pJson, const char* _strKey, const T& _srcT)
	{
		using valType = T::value_type;

		//벡터(또는 배열) + 포인터일것
		static_assert(
			true == std::is_pointer_v<valType> ||
			true == std::is_smart_ptr_v<valType>
			);

		//Value가 들어있는 vector 저장하는법
		//1. json의 타입을 array value로 만들어준다.
		(*_pJson)[_strKey] = Json::Value(Json::arrayValue);

		//2. 만들어진 데이터 컨테이너를 레퍼런스로 받아온다.
		Json::Value& jVal = (*_pJson)[_strKey];
		for (size_t i = 0; i < _srcT.size(); ++i)
		{
			if (_srcT[i])
			{
				jVal.append(_srcT[i]->GetKey());
			}
		}
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


	template <typename T>
	std::string MHLoadPtr(const Json::Value* _pJson, const char* _strKey, const T& _destT)
	{
		std::string resultStr{};
		if (_pJson->isMember(_strKey))
		{
			resultStr = _pJson[_strKey].asString();
		}

		return resultStr;
	}


	template <typename T, typename std::enable_if_t<(
		true == std::is_vector_v<T> ||
		true == std::is_array_v<T>
		), bool>* = nullptr>
	static T MHGetJsonVector(const Json::Value* _pJson, const char* _strKey, const T& _tSrc)
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


	static std::vector<std::string> MHGetJsonVectorPtr(const Json::Value* _pJson, const char* _strKey)
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

#define JSON_KEY(_varName) #_varName
#define JSON_KEY_PAIR(_varName) #_varName, _varName


