#pragma once

#ifndef JSON_AMALGAMATED_H_INCLUDED
#include "json.h"
#endif
#include "../define_Util.h"
#include "../define_Enum.h"
#include <wrl.h>
#include <utility>


namespace Json
{
	template <typename T, typename = void>
	struct need_base64 : std::true_type {};

	template <typename T>
	struct need_base64<T, std::enable_if_t<std::is_integral_v<T>>> : std::false_type {};

	template <typename T>
	struct need_base64<T, std::enable_if_t<std::is_same_v<std::string, T>>> : std::false_type {};

	template <typename T>
	struct need_base64<T, std::enable_if_t<std::is_enum_v<T>>> : std::false_type {};

	template <typename T>
	struct need_base64<T, std::enable_if_t<std::is_pointer_v<T>>> : std::false_type {};

	template <typename T>
	struct need_base64<T, std::enable_if_t<std::is_smart_ptr_v<T>>> : std::false_type {};

	template <typename T>
	inline constexpr bool need_base64_v = need_base64<T>::value;

	class MH
	{
	public:
		template <typename T, typename std::enable_if_t<(
			false == need_base64_v<T> &&
			(std::is_same_v<std::string, T> || std::is_integral_v<T>)
			)
			, bool>* = nullptr>
		static T ConvertWrite(const T& _srcT)
		{
			return _srcT;
		}

		template <typename T, typename std::enable_if_t<(
			false == need_base64_v<T> &&
			std::is_enum_v<T>)>* = nullptr>
		static int ConvertWrite(const T& _srcT)
		{
			return (int)_srcT;
		}

		template <typename T, typename std::enable_if_t<(
			false == need_base64_v<T> &&
			(std::is_pointer_v<T> || std::is_smart_ptr_v<T>)
			)>* = nullptr>
		static std::string ConvertWrite(const T& _srcT)
		{
			return _srcT->GetKey();
		}

		template <typename T, typename
			std::enable_if_t <need_base64_v<T>, bool>* = nullptr>
		static std::string ConvertWrite(const T& _srcT)
		{
			//컨테이너는 변환 불가
			static_assert(false == std::is_container_v<T>);
			return StringConv::Convert_T_to_String(_srcT);
		}




		template <typename T, typename std::enable_if_t<
			false == need_base64_v<T> &&
			(std::is_same_v<std::string, T> || std::is_integral_v<T>)
			, bool>* = nullptr>
		static T ConvertRead(const Json::Value& _jVal)
		{
			return _jVal.as<T>();
		}

		template <typename T, typename std::enable_if_t<
			(false == need_base64_v<T> && std::is_enum_v<T>)
			, bool>* = nullptr>
		static T ConvertRead(const Json::Value& _jVal)
		{
			return (T)_jVal.asInt();
		}

		template <typename T, typename std::enable_if_t<(
			false == need_base64_v<T> &&
			(std::is_pointer_v<T> || std::is_smart_ptr_v<T>)
			)>* = nullptr>
		static std::string ConvertRead(const Json::Value& _jVal)
		{
			return _jVal.asString();
		}

		template <typename T, typename
			std::enable_if_t < (need_base64_v<T>)
			, bool>* = nullptr>
		static T ConvertRead(const Json::Value& _jVal)
		{
			//컨테이너는 변환 불가
			static_assert(false == std::is_container_v<T>);
			return StringConv::Convert_String_to_T<T>(_jVal.asString());
		}


		template <typename T>
		static  void SaveValue(Json::Value* _pJson, const char* _strKey, const T& _srcT)
		{
			Json::Value& jVal = (*_pJson);
			jVal[_strKey] = ConvertWrite<T>(_srcT);
		}

		template <typename T>
		static void SaveStrKey(Json::Value* _pJson, const char* _strKey, const T& _srcT)
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
		static void SaveValueVector(Json::Value* _pJson, const char* _strKey, const T& _srcT)
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
				jVal.append(ConvertWrite(_srcT[i]));
			}
		}


		//GetKey()가 있는 클래스 포인터 한정으로 사용가능
		template <typename T, typename std::enable_if_t<
			(true == std::is_vector_v<T> ||
				true == std::is_std_array_v<T>)
			, int>* = nullptr>
		static void SaveStrKeyVector(Json::Value* _pJson, const char* _strKey, const T& _srcT)
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
				else
				{
					jVal.append("");
				}
			}
		}



		template <typename T>
		static bool LoadValue(const Json::Value* _pJson, const char* _strKey, T& _destT)
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
					_destT = StringConv::Convert_String_to_T<T>(jVal.asString());
					bResult = true;
				}
			}
			return bResult;
		}

		template <typename T>
		static std::string LoadStrKey(const Json::Value* _pJson, const char* _strKey, const T& _destT)
		{
			std::string resultStr{};
			if (_pJson->isMember(_strKey))
			{
				resultStr = (*_pJson)[_strKey].asString();
			}

			return resultStr;
		}

		template <typename T, typename std::enable_if_t<(
			true == std::is_vector_v<T> ||
			true == std::is_array_v<T>
			), bool>* = nullptr>
		static T LoadValueVector(const Json::Value* _pJson, const char* _strKey, const T& _tSrc)
		{
			using valType = T::value_type;

			//포인터에 대한 값은 추가할 수 없음.
			static_assert(false == std::is_pointer_v<valType> || false == std::is_smart_ptr_v<valType>);

			T vecT{};
			if (_pJson->isMember(_strKey))
			{
				const Json::Value& jVal = (*_pJson)[_strKey];
				if (jVal.isArray())
				{
					for (Json::ValueConstIterator iter = jVal.begin(); iter != jVal.end(); ++iter)
					{
						vecT.push_back(ConvertRead<valType>(*iter));
					}
				}
			}
			return vecT;
		}


		static std::vector<std::string> LoadStrKeyVector(const Json::Value* _pJson, const char* _strKey)
		{
			std::vector<std::string> retVec;

			if (_pJson->isMember(_strKey))
			{
				const Json::Value& jVal = (*_pJson)[_strKey];
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

	private:

		MH() = delete;
		~MH() = delete;
	};

	////Value가 들어있는 map만
	//template <typename T, typename std::enable_if_t<(
	//	true == std::is_pair_container_v<T>
	//	), bool>* = nullptr>
	//static T MHGetJsonMap(const Json::Value* _pJson, const char* _strKey)
	//{
	//	using keyType = T::key_type;
	//	using valType = T::mapped_type;
	//	static_assert(false == std::is_pointer_v<keyType> || false == std::is_smart_ptr_v<keyType>);
	//	static_assert(false == std::is_pointer_v<valType> || false == std::is_smart_ptr_v<valType>);

	//	T vecT{};
	//	if (_pJson->isMember(_strKey))
	//	{
	//		const Json::Value& jVal = (*_pJson)[_strKey];
	//		for (Json::ValueConstIterator iter = jVal.begin(); iter != jVal.end(); ++iter)
	//		{
	//			keyType key = ConvertRead<keyType>(iter.key());
	//			valType val = ConvertRead<valType>(*iter);
	//			vecT.insert(std::make_pair(key, val));
	//		}
	//	}
	//	return vecT;
	//}
}

#define JSON_KEY(_varName) #_varName

#define JSON_KEY_PAIR(_varName) #_varName, _varName


