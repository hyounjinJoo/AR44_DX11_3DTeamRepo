#pragma once
#include "Entity.h"
#include "define_Struct.h"

//MeshData에 종속된 클래스
namespace mh
{
	class FBXLoader;
	class StructBuffer;
	class Skeleton :
		public Entity
	{
	public:
		Skeleton();
		virtual ~Skeleton();

		define::eResult Save(const std::filesystem::path& _fullPath);
		define::eResult Load(const std::filesystem::path& _fullPath);

		define::eResult CreateFromFBX(FBXLoader* _pFBXLoader);

	public:
		//Animation 3D
		const std::vector<define::tMTBone>* GetBones() { return &m_vecBones; }
		UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
		const std::vector<define::tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
		bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

		StructBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData.get(); } // 전체 본 프레임 정보
		StructBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset.get(); }	   // 각 뼈의 offset 행렬

	private:
		std::vector<define::tMTBone>			m_vecBones;
		std::unique_ptr<StructBuffer>			m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)

		//Animation3D 정보
		std::vector<define::tMTAnimClip>		m_vecAnimClip;
		std::unique_ptr<StructBuffer>			m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
	};
}


