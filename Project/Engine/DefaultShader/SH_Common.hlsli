#ifndef SH_COMMON
#define SH_COMMON


//C++
#ifdef __cplusplus

#include "../SimpleMath.h"

namespace mh::math
{
using float2 = Vector2;
using float3 = Vector2;
using float4 = Vector2;

using BOOL = int;
//윈도우 쪽 헤더에 정의되어 있음
//constexpr const int TRUE = 1;
//constexpr const int FALSE = 0;


struct int2 { int x; int y; };
struct int3 { int x; int y; int z; };
struct int4 { int x; int y; int z; int w; };

using uint = UINT;
struct uint2 { uint x; uint y; };
struct uint3 { uint x; uint y; uint z; };
struct uint4 { uint x; uint y; uint z; uint w; };
}

using MATRIX = Matrix;

#define SEMANTIC(_Type)

#define cbuffer struct

//=========================================
//================ HLSL ===================
//=========================================
#else

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



#endif
