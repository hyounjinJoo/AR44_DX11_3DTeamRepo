#pragma once
#include <string>
#include <vector>
#include <Windows.h>

#include "DefaultShader/SH_CommonStruct.hlsli"
#include "SimpleMath.h"
#include "define_Enum.h"
#include <fbxsdk/core/base/fbxtime.h>

//C++ 전용 '구조체' 모음
namespace mh::define
{
	struct tDesc_GPUMgr
	{
		UINT ResolutionX;
		UINT ResolutionY;
		UINT RefreshRate;
	};

	struct tDesc_Application
	{
		HWND Hwnd;
		int  LeftWindowPos;
		int	 TopWindowPos;
		int  Width;
		int  Height;
		tDesc_GPUMgr GPUDesc;
		WNDCLASSEX WinClassDesc;
	};

	struct tUmap_LightHashFunc32
	{
		UINT64 operator()(const UINT32& _ukey) const
		{
			return static_cast<UINT64>(_ukey);
		}
	};

	struct tUmap_LightHashFunc64
	{
		UINT64 operator()(const UINT64& _ukey) const
		{
			return static_cast<UINT64>(_ukey);
		}
	};

	struct tUmap_StringViewHasher
	{
		using hash_type = std::hash<std::string_view>;
		using is_transparent = void;

		std::size_t operator()(const char* str) const { return hash_type{}(str); }
		std::size_t operator()(const std::string_view str) const { return hash_type{}(str); }
		std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
	};

	struct tDataPtr
	{
		void* pData;
		size_t size;

		template <typename T>
		void SetDataPtr(const T _pData) { pData = (void*)_pData; size = sizeof(T); }
	};

	struct tMassProperties
	{
		tMassProperties(float _staticFriction = 0.5f, float _dynamicFriction = 0.2f, float _restitution = 0.603f)
			: staticFriction(_staticFriction)
			, dynamicFriction(_dynamicFriction)
			, restitution(_restitution)
		{ }

		float staticFriction;
		float dynamicFriction;
		float restitution;
	};

	struct Geometries
	{
		Geometries(eGeometryType _geometryType, float3 vBoxHalfSize)
			: eGeomType(eGeometryType::Box)
		{
			if (eGeometryType::Box == _geometryType)
			{
				boxGeom = physx::PxBoxGeometry(vBoxHalfSize);
			}
		}

		Geometries(eGeometryType _geometryType, float fRadius, float fHalfHeight)
			: eGeomType(eGeometryType::Capsule)
		{
			if (eGeometryType::Capsule == _geometryType)
			{
				capsuleGeom = physx::PxCapsuleGeometry(fRadius, fHalfHeight);
			}
		}

		Geometries(eGeometryType _geometryType, float fRadius)
			: eGeomType(eGeometryType::Sphere)
		{
			if (eGeometryType::Sphere == _geometryType)
			{
				sphereGeom = physx::PxSphereGeometry(fRadius);
			}
		}

		Geometries(eGeometryType _geometryType)
			: eGeomType(eGeometryType::Plane)
		{
			// RigidStatic일 떄,
			if (eGeometryType::Plane == _geometryType)
			{
				planeGeom = physx::PxPlaneGeometry();
			}
		}

		physx::PxBoxGeometry boxGeom;
		physx::PxCapsuleGeometry capsuleGeom;
		physx::PxPlaneGeometry planeGeom;
		physx::PxSphereGeometry sphereGeom;
		define::eGeometryType eGeomType;
	};

	struct tPhysicsInfo
	{
		tPhysicsInfo()
			: eActorType(eActorType::Static)
			, eGeomType(eGeometryType::Box)
			, size(float3(1.f, 1.f, 1.f))
			, massProperties(tMassProperties())
			, pGeometries(nullptr)
			, filterData{}
		{
		}

		eActorType eActorType;
		eGeometryType eGeomType;
		float3 size;
		tMassProperties massProperties;
		Geometries* pGeometries;
		physx::PxFilterData filterData;
	};


// ============
// Animation 3D
// ============
	//struct tAnimKeyframeTranslation
	//{
	//	float4	vTranslate;
	//	float4	vScale;
	//	float4	qRot;
	//};

	struct tMTKeyFrame
	{
		double	dTime;
		int		iFrame;
		tAnimKeyframeTranslation  FrameTrans;
	};

	struct tMTBone
	{
		std::string			strBoneName;

		struct Value//저장을 위해서 별도의 struct 안에 넣어놓음
		{
			int					iDepth;
			int					iParentIndx;
			MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
			MATRIX				matBone;   // 이거 안씀
		} Val;

		std::vector<tMTKeyFrame>	vecKeyFrame;
	};

	struct tMTAnimClip
	{
		std::string			strAnimName;
		struct Value//저장을 위해서 별도의 struct 안에 넣어놓음
		{
			int				iStartFrame;
			int				iEndFrame;
			int				iFrameLength;

			double			dStartTime;
			double			dEndTime;
			double			dTimeLength;
			float			fUpdateTime; // 이거 안씀

			fbxsdk::FbxTime::EMode	eMode;
		} Val;
	};


	struct tDebugMesh
	{
		define::eColliderType type;
		float3 position;
		float3 rotatation;
		float3 scale;

		float radius;
		float duration;
		float time;
	};

	//struct tLightAttribute
	//{
	//	float4 diffuse;
	//	float4 specular;
	//	float4 ambient;

	//	float4 position;
	//	float4 direction;

	//	define::eLightType type;
	//	float radius;
	//	float angle;
	//	int padding;
	//};

	//struct tParticle
	//{
	//	float4 position;
	//	float4 direction;

	//	float lifeTime;
	//	float time;
	//	float speed;
	//	uint active;
	//};
	//struct tParticleShared
	//{
	//	uint activeCount;
	//};
}
