#include "PCH_Engine.h"
#include "Com_Animator3D.h"

#include "TimeMgr.h"
#include "Com_Renderer_Mesh.h"
#include "StructBuffer.h"
#include "ResMgr.h"
#include "InputMgr.h"

#include "Animation3DShader.h"
#include "GameObject.h"
#include "Skeleton.h"
#include "Animation3D.h"

namespace mh
{
	using namespace mh::define;

	Com_Animator3D::Com_Animator3D()
		: IAnimator(define::eDimensionType::_3D)
		//, m_pVecBones(nullptr)
		//, m_pVecClip(nullptr)
		, mSkeleton()
		, m_dCurTime(0.)
		, m_iFramePerSecond(30)
		, m_pBoneFinalMatBuffer(nullptr)
		, m_bFinalMatUpdate(false)
		, m_iFrameIdx(0)
		, m_iNextFrameIdx(0)
		, m_fRatio(0.f)
		, mCurrentAnim()
		, m_fClipUpdateTime()
	{
		m_pBoneFinalMatBuffer = std::make_unique<StructBuffer>();
	}

	Com_Animator3D::Com_Animator3D(const Com_Animator3D& _other)
		: IAnimator(_other)
		//, m_pVecBones(_other.m_pVecBones)
		//, m_pVecClip(_other.m_pVecClip)
		, mSkeleton()
		, m_dCurTime(_other.m_dCurTime)
		, m_iFramePerSecond(_other.m_iFramePerSecond)
		, m_pBoneFinalMatBuffer(nullptr)
		, m_bFinalMatUpdate(false)
		, m_iFrameIdx(_other.m_iFrameIdx)
		, m_iNextFrameIdx(_other.m_iNextFrameIdx)
		, m_fRatio(_other.m_fRatio)
		, mCurrentAnim(_other.mCurrentAnim)
		, m_fClipUpdateTime()
	{
		if (_other.m_pBoneFinalMatBuffer)
		{
			m_pBoneFinalMatBuffer = std::unique_ptr<StructBuffer>(_other.m_pBoneFinalMatBuffer->Clone());
		}
	}

	Com_Animator3D::~Com_Animator3D()
	{
	}

	void Com_Animator3D::Init()
	{
		tSBufferDesc desc{};
		desc.REGISLOT_t_SRV = Register_t_g_BoneMatrixArray;
		desc.TargetStageSRV = eShaderStageFlag::VS;
		desc.REGISLOT_u_UAV = Register_u_g_FinalBoneMatrixArray;
		desc.eSBufferType = eStructBufferType::READ_WRITE;

		m_pBoneFinalMatBuffer->SetDesc(desc);
	}


	void Com_Animator3D::FixedUpdate()
	{
		//애니메이션이 없을 경우 플레이하지 않는다
		if (nullptr == mSkeleton || nullptr == mCurrentAnim)
			return;

		//const auto& animClips = mSkeleton->GetAnimations();

		m_dCurTime = 0.f;
		// 현재 재생중인 Clip 의 시간을 진행한다.
		m_fClipUpdateTime += TimeMgr::DeltaTime();

		//한 프레임 시간이 지나갔으면 다음 프레임으로
		float frameTime = (float)mCurrentAnim->GetTimeLength();
		if (m_fClipUpdateTime >= frameTime)
		{
			m_fClipUpdateTime = 0.f;
		}

		m_dCurTime = mCurrentAnim->GetStartTime() + (double)m_fClipUpdateTime;

		// 현재 프레임 인덱스 구하기
		double dFrameIdx = m_dCurTime * (double)m_iFramePerSecond;
		m_iFrameIdx = (int)dFrameIdx;

		//만약 이미 마지막 프레임에 도달했을 경우 현재 프레임 유지
		int maxFrameCount = mCurrentAnim->GetFrameLength();
		if (m_iFrameIdx >= maxFrameCount - 1)
			m_iNextFrameIdx = maxFrameCount - 1;	// 끝이면 현재 인덱스를 유지
		else
			m_iNextFrameIdx = m_iFrameIdx + 1;

		// 프레임간의 시간에 따른 비율을 구해준다.
		m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

		// 컴퓨트 쉐이더 연산여부
		m_bFinalMatUpdate = false;
	}
	

	void Com_Animator3D::SetSkeleton(std::shared_ptr<Skeleton> _pSkeleton)
	{
		//스켈레톤 주소를 받아서
		mSkeleton = _pSkeleton;
		if (mSkeleton)
		{
			//최종 Bone별 행렬이 저장될 Vector 크기를 재조정
			m_vecFinalBoneMat.resize(mSkeleton->GetBoneCount());
		}
	}

	bool Com_Animator3D::Play(const std::string& _strAnimName)
	{
		bool isPlayed = false;
		if (mSkeleton)
		{
			mCurrentAnim = mSkeleton->FindAnimation(_strAnimName);
			if (mCurrentAnim)
			{
				m_iFramePerSecond = mCurrentAnim->GetFPS();
				isPlayed = true;
			}
		}

		return isPlayed;
	}


	void Com_Animator3D::BindData()
	{
		if (false == m_bFinalMatUpdate)
		{
			// Animation3D Update Compute Shader
			static std::shared_ptr<Animation3DShader> pUpdateShader = ResMgr::Load<Animation3DShader>(define::strKey::Default::shader::compute::Animation3D);


			//구조화 버퍼가 정상적으로 생성되었는지 확인한다.
			if (false == CheckMesh())
				return;
			
			pUpdateShader->SetFrameDataBuffer(mCurrentAnim->GetKeyFrameSBuffer());
			pUpdateShader->SetOffsetMatBuffer(mSkeleton->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer.get());

			UINT iBoneCount = (UINT)mSkeleton->GetBoneCount();
			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetCurFrameIdx(m_iFrameIdx);
			pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
			pUpdateShader->SetFrameRatio(m_fRatio);
			pUpdateShader->SetFrameLength(mCurrentAnim->GetFrameLength());


			// 업데이트 쉐이더 실행
			pUpdateShader->OnExcute();

			m_bFinalMatUpdate = true;
		}

		// t19 레지스터에 최종행렬 데이터(구조버퍼) 바인딩	
		m_pBoneFinalMatBuffer->BindDataSRV();// Register_t_g_arrBoneMat, eShaderStageFlag::VS);
	}


	void Com_Animator3D::UnBindData()
	{
		m_pBoneFinalMatBuffer->UnBindData();
	}




	bool Com_Animator3D::CheckMesh()
	{
		bool result = false;
		if (mSkeleton)
		{
			UINT iBoneCount = mSkeleton->GetBoneCount();
			if (m_pBoneFinalMatBuffer->GetElemCount() != iBoneCount)
			{
				if(SUCCEEDED(m_pBoneFinalMatBuffer->Create<MATRIX>((size_t)iBoneCount, nullptr, 0)))
					result = true;
			}
		}
		return result;
	}
}
