#pragma once
#include "IAnimator.h"

#include "define_Struct.h"

#include <fbxsdk/fbxsdk.h>

namespace mh
{
	class StructBuffer;
	class Mesh;
	class Skeleton;
	class Animation3D;
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

		void SetSkeleton(std::shared_ptr<Skeleton> _pSkeleton);
		std::shared_ptr<const Skeleton> GetSkeleton() const { return mSkeleton; }
		bool Play(const std::string& _strAnimName);

		void PlayNext();

		//void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

		StructBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer.get(); }
		//UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }

		virtual void BindData() override;
		virtual void UnBindData() override;
	

	private:
		bool Play(std::shared_ptr<Animation3D> _anim);

		bool CheckMesh();

    private:
		std::shared_ptr<Skeleton>						mSkeleton;
		std::shared_ptr<Animation3D>					mCurrentAnim;
		std::shared_ptr<Animation3D>					mNextAnim;

        std::vector<MATRIX>				m_vecFinalBoneMat;		// 텍스쳐에 전달할 최종 행렬정보

		tCB_Animation3D					m_Anim3DCBuffer;

		//현재 프레임
        int								m_iFramePerSecond;
        double							m_dCurTime;
		float							m_fClipUpdateTime;
		

		//다음 애니메이션 전환 여부
		bool							m_bChangeAnim;

		//전체 시간 길이
		float							m_fChangeTimeLength;

		//변경 시간 누적
		float							m_fChangeTimeAccumulate;


		

        std::unique_ptr<StructBuffer>	m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
        bool							m_bFinalMatUpdate;		// 최종행렬 연산 수행여부
	};
}


