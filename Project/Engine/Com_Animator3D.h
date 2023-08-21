#pragma once
#include "IAnimator.h"

#include "define_Struct.h"

#include <fbxsdk/fbxsdk.h>

namespace mh
{
	class StructBuffer;
	class Mesh;
	class Skeleton;
	class Com_Animator3D :
		public IAnimator
	{
	public:
		Com_Animator3D();

		Com_Animator3D(const Com_Animator3D& _other);
		CLONE(Com_Animator3D);

		virtual ~Com_Animator3D();

		virtual void Init() override;
		virtual void Update() {};
		virtual void FixedUpdate() override;

		void SetSkeleton(Skeleton* _pSkeleton);
		const Skeleton* GetSkeleton() const { return mSkeleton; }

		void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

		StructBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
		//UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }

		virtual void BindData() override;
		virtual void UnBindData() override;

	private:
		bool CheckMesh();

    private:
		Skeleton* mSkeleton;
        //const std::vector<define::tMTBone>* m_pVecBones;
        //const std::vector<define::tMTAnimClip>* m_pVecClip;

        std::vector<float>				m_vecClipUpdateTime;
        std::vector<MATRIX>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
        int								m_iFrameCount; // 30
        double							m_dCurTime;
        int								m_iCurClip; // 클립 인덱스	

        int								m_iFrameIdx; // 클립의 현재 프레임
        int								m_iNextFrameIdx; // 클립의 다음 프레임
        float							m_fRatio;	// 프레임 사이 비율

        StructBuffer*					m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
        bool							m_bFinalMatUpdate; // 최종행렬 연산 수행여부
	};
}


