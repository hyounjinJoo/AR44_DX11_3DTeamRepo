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

namespace mh
{
	using namespace mh::define;

	Com_Animator3D::Com_Animator3D()
		: IAnimator(define::eDimensionType::_3D)
		, m_pVecBones(nullptr)
		, m_pVecClip(nullptr)
		, m_iCurClip(0)
		, m_dCurTime(0.)
		, m_iFrameCount(30)
		, m_pBoneFinalMatBuffer(nullptr)
		, m_bFinalMatUpdate(false)
		, m_iFrameIdx(0)
		, m_iNextFrameIdx(0)
		, m_fRatio(0.f)
	{
		m_pBoneFinalMatBuffer = new StructBuffer;
	}

	Com_Animator3D::Com_Animator3D(const Com_Animator3D& _origin)
		: IAnimator(_origin)
		, m_pVecBones(_origin.m_pVecBones)
		, m_pVecClip(_origin.m_pVecClip)
		, m_iCurClip(_origin.m_iCurClip)
		, m_dCurTime(_origin.m_dCurTime)
		, m_iFrameCount(_origin.m_iFrameCount)
		, m_pBoneFinalMatBuffer(nullptr)
		, m_bFinalMatUpdate(false)
		, m_iFrameIdx(_origin.m_iFrameIdx)
		, m_iNextFrameIdx(_origin.m_iNextFrameIdx)
		, m_fRatio(_origin.m_fRatio)
	{
		m_pBoneFinalMatBuffer = new StructBuffer;
	}

	Com_Animator3D::~Com_Animator3D()
	{
		if (m_pBoneFinalMatBuffer)
			delete m_pBoneFinalMatBuffer;
	}


	void Com_Animator3D::FixedUpdate()
	{
		m_dCurTime = 0.f;
		// 현재 재생중인 Clip 의 시간을 진행한다.
		m_vecClipUpdateTime[m_iCurClip] += TimeMgr::DeltaTime();

		if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).Val.dTimeLength)
		{
			m_vecClipUpdateTime[m_iCurClip] = 0.f;
		}

		m_dCurTime = m_pVecClip->at(m_iCurClip).Val.dStartTime + m_vecClipUpdateTime[m_iCurClip];

		// 현재 프레임 인덱스 구하기
		double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
		m_iFrameIdx = (int)(dFrameIdx);

		// 다음 프레임 인덱스
		if (m_iFrameIdx >= m_pVecClip->at(0).Val.iFrameLength - 1)
			m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
		else
			m_iNextFrameIdx = m_iFrameIdx + 1;

		// 프레임간의 시간에 따른 비율을 구해준다.
		m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

		// 컴퓨트 쉐이더 연산여부
		m_bFinalMatUpdate = false;
	}

	void Com_Animator3D::SetBones(Skeleton* _pSkeleton)
	{
		//TODO: 여기 구현
		MH_ASSERT(nullptr);
		//m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size());
	}

	void Com_Animator3D::SetAnimClip(const std::vector<tMTAnimClip>* _vecAnimClip)
	{
		m_pVecClip = _vecAnimClip;
		m_vecClipUpdateTime.resize(m_pVecClip->size());

		// 테스트 코드
		/*static float fTime = 0.f;
		fTime += 1.f;
		m_vecClipUpdateTime[0] = fTime;*/
	}


	void Com_Animator3D::BindGPU()
	{
		if (false == m_bFinalMatUpdate)
		{
			// Animation3D Update Compute Shader
			std::shared_ptr<Animation3DShader> pUpdateShader = ResMgr::Load<Animation3DShader>(define::strKey::Default::shader::compute::Animation3D);

			// Bone Data
			std::shared_ptr<Mesh> pMesh = GetOwner()->GetComponent<Com_Renderer_Mesh>()->GetMesh();

			//메쉬와 본 하나라도 없을 경우에는 애니메이션을 재생할 수 없다
			if (false == (pMesh && pMesh->GetSkeleton()))
			{
				return;
			}

			//구조화 버퍼가 정상적으로 생성되었는지 확인한다.
			check_mesh(pMesh);
			
			Skeleton* pBone = pMesh->GetSkeleton();
			pUpdateShader->SetFrameDataBuffer(pBone->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pBone->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

			UINT iBoneCount = (UINT)m_pVecBones->size();
			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_iFrameIdx);
			pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
			pUpdateShader->SetFrameRatio(m_fRatio);

			// 업데이트 쉐이더 실행
			pUpdateShader->OnExcute();

			m_bFinalMatUpdate = true;
		}

		// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩	
		m_pBoneFinalMatBuffer->BindDataSRV(Register_t_g_arrBoneMat, eShaderStageFlag::VS);
	}


	void Com_Animator3D::Clear()
	{
		m_pBoneFinalMatBuffer->UnBind();

		Com_Renderer_Mesh* MeshRenderer = GetOwner()->GetComponent<Com_Renderer_Mesh>();
		UINT iMtrlCount = MeshRenderer->GetMaterialCount();
		Material* pMtrl = nullptr;
		for (UINT i = 0; i < iMtrlCount; ++i)
		{
			//TODO: 43기에서는 Shared Material을 받고 있음.
			pMtrl = MeshRenderer->GetCurrentMaterial(i);

			if (nullptr == pMtrl)
				continue;

			pMtrl->SetAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	void Com_Animator3D::Init()
	{
		tSBufferDesc desc{};
		desc.REGISLOT_u_UAV = Register_u_g_arrFinalMat;
		desc.eSBufferType = eStructBufferType::READ_WRITE;
		
		m_pBoneFinalMatBuffer->SetDesc(desc);
	}


	void Com_Animator3D::check_mesh(std::shared_ptr<Mesh> _pMesh)
	{
		if (_pMesh && _pMesh->GetSkeleton())
		{
			UINT iBoneCount = _pMesh->GetSkeleton()->GetBoneCount();
			if (m_pBoneFinalMatBuffer->GetElemCount() != iBoneCount)
			{
				m_pBoneFinalMatBuffer->Create<MATRIX>((size_t)iBoneCount, nullptr, 0);
			}
		}
	}
}
