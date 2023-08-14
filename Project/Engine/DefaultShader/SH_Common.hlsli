#ifndef SH_COMMON
#define SH_COMMON

//C++
#ifdef __cplusplus


using float2 = mh::math::Vector2;
using float3 = mh::math::Vector3;
using float4 = mh::math::Vector4;
using MATRIX = mh::math::Matrix;
using uint = UINT;

//윈도우 쪽 헤더에 정의되어 있음
#ifndef BOOL
using BOOL = int;
#endif

#ifndef TRUE
constexpr const int TRUE = 1;
#endif
#ifndef FALSE
constexpr const int FALSE = 0;
#endif

struct int2 { int x; int y; };
struct int3 { int x; int y; int z; };
struct int4 { int x; int y; int z; int w; };

using uint = UINT;
struct uint2 { uint x; uint y; };
struct uint3 { uint x; uint y; uint z; };
struct uint4 { uint x; uint y; uint z; uint w; };



#define SEMANTIC(_Type)

#else
//=========================================
//================ HLSL ===================
//=========================================

#define FLT_EPSILON 1.192092896e-07F

#define BOOL int
#define TRUE 1
#define FALSE 0
#define MATRIX row_major matrix

#define SEMANTIC(_Type)  : _Type

#endif

//ENUM 관련
#ifdef __cplusplus

//C++ : enum class 형태로 선언함.
#define ENUM_BEGIN(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

//HLSL : namespace 형태로 선언함.
#define ENUM_BEGIN(_Name, _Type) namespace _Name {
#define ENUM_MEMBER(_Name, _Type, _Val) static const _Type _Name = _Val;
#define ENUM_END };

#endif


//Register Slot 관련
#ifdef __cplusplus

#define CBUFFER(_bufferName, _structName, _registerNumber)\
constexpr int CBuffer_##_bufferName = _registerNumber;\
constexpr int Register_b_##_bufferName = _registerNumber

#define TEXTURE2D(_bufferName, _structName, _registerNumber)


#define REGISTER_DECLARE(_bufferType, _bufferName, _registerType, _registerNumber)\
constexpr int register_##_registerType##_##_bufferName = _registerNumber;\
_bufferType(_bufferType, _bufferName)

#define cbuffer(_bufferType, _bufferName) struct alignas(16) _bufferName
#define Texture2D(_bufferType, _bufferName) 

#define REGISTER(_Type)
#define REGISTER_SLOT(_RegisterType, _iSlotIdx) _iSlotIdx

#else//HLSL

#define CBUFFER(_bufferName, _structName, _registerNumber)\
cbuffer _bufferName : register(b##_registerNumber)\
{ _structName _bufferName; }




#define REGISTER_DECLARE(_bufferType, _bufferName, _registerType, _registerNumber)\
_bufferType _bufferName : register(_registerType##_registerNumber)

#define alignas(_Num)

#define REGISTER(_Type) : register(_Type)
#define REGISTER_SLOT(_RegisterType, _iSlotIdx) _RegisterType##_iSlotIdx

#endif

#endif



