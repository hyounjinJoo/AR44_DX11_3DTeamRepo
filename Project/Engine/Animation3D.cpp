#include "PCH_Engine.h"
#include "Animation3D.h"
#include "FBXLoader.h"
#include "StructBuffer.h"

namespace mh
{
	Animation3D::Animation3D()
        : mAnimationName{}
        , m_StartTime{}
		, m_EndTime{}
		, m_TimeLength{}
		, m_FrameTime{}
		, m_PlayTime{}
		, m_PlayScale{}
		, m_StartFrame{}
		, m_EndFrame{}
		, m_FrameCount{}
		, m_FrameMode{}
		, m_ChangeFrame{}
		, m_End{}
		, mKeyFramesPerBone{}
		, m_KeyFrameBuffer{}
	{
	}
	Animation3D::~Animation3D()
	{
	}
    void Animation3D::BindData()
    {
        MH_ASSERT(m_KeyFrameBuffer);
        m_KeyFrameBuffer->BindDataSRV();
    }
    void Animation3D::UnBindData()
    {
        m_KeyFrameBuffer->UnBindData();
    }
	eResult Animation3D::LoadFromFBX(const tFBXAnimClip* _clip)
	{
		if (nullptr == _clip)
			return eResult::Fail_Nullptr;

		mAnimationName = _clip->strName;

		switch (_clip->TimeMode)
		{
		case fbxsdk::FbxTime::eFrames24:
			m_FrameMode = 24;
			break;
		case fbxsdk::FbxTime::eFrames30:
			m_FrameMode = 30;
			break;
		case fbxsdk::FbxTime::eFrames60:
			m_FrameMode = 60;
			break;
		}

        // FbxAnimationClip에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
        m_StartFrame = (int)_clip->StartTime.GetFrameCount(_clip->TimeMode);
        m_EndFrame = (int)_clip->EndTime.GetFrameCount(_clip->TimeMode);
        m_FrameCount = (int)(m_EndFrame - m_StartFrame + 1);

        // 시간 정보를 저장해준다.
        m_StartTime = 0.f;
        m_EndTime = m_PlayTime;
        m_TimeLength = m_PlayTime;

        m_FrameTime = m_PlayTime / m_FrameCount;




        //GPU로 보낼 데이터
        std::vector<tAnimFrameTranslation>	vecFrameTrans;

        //GPU로 보낼 데이터를 작성
        //GPU는 이중 배열 같은걸 지원 안함
        //그러니까 1차원 배열 형태로 애니메이션 데이터를 보내야 함
        // 본 3개 키프레임 5개일 경우
        // 본0키0|본0키1|본0키2|본0키3|본0키4|
        // 본1키0|본1키1|본1키2|본1키3|본1키4|
        // 본2키0|본2키1|본2키2|본2키3|본2키4
        //그러므로 하나의 애니메이션은 본의 갯수 * 키프레임 갯수가 된다
        vecFrameTrans.resize(_clip->KeyFramesPerBone.size() * (size_t)m_FrameCount);


        // 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로 뽑아온다.
        mKeyFramesPerBone.reserve(_clip->KeyFramesPerBone.size());
        for (size_t i = 0; i < _clip->KeyFramesPerBone.size(); ++i)
        {
            mKeyFramesPerBone.push_back(tKeyFramesPerBone{});
            tKeyFramesPerBone& keyFramesPerBone = mKeyFramesPerBone.back();

            keyFramesPerBone.BoneIndex = _clip->KeyFramesPerBone[i].BoneIndex;

            // 아래부터 키프레임 정보를 저장한다.
            keyFramesPerBone.vecKeyFrame.reserve(_clip->KeyFramesPerBone[i].vecKeyFrame.size());
            for (size_t j = 0; j < _clip->KeyFramesPerBone[i].vecKeyFrame.size(); ++j)
            {
                keyFramesPerBone.vecKeyFrame.push_back(tKeyFrame{});
                tKeyFrame& keyFrame = keyFramesPerBone.vecKeyFrame.back();
                keyFrame.Time = j * m_FrameTime;

                // 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
                fbxsdk::FbxAMatrix		mat = _clip->KeyFramesPerBone[i].vecKeyFrame[j].matTransform;

                // 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
                fbxsdk::FbxVector4 Pos = mat.GetT();
                fbxsdk::FbxVector4 Scale = mat.GetS();
                fbxsdk::FbxQuaternion Rot = mat.GetQ();

                keyFrame.Scale = 
                    float3((float)Scale.mData[0], (float)Scale.mData[1], (float)Scale.mData[2]);
                keyFrame.Pos = 
                    float3((float)Pos.mData[0], (float)Pos.mData[1], (float)Pos.mData[2]);
                keyFrame.RotQuat = 
                    float4((float)Rot.mData[0], (float)Rot.mData[1], (float)Rot.mData[2], (float)Rot.mData[3]);


                //GPU에 보낼 데이터 생성
                if (j < (size_t)m_FrameCount)
                {
                    vecFrameTrans[i * m_FrameCount + j].vScale =
                        float4(keyFrame.Scale.x, keyFrame.Scale.y, keyFrame.Scale.z, 1.f);

                    vecFrameTrans[i * m_FrameCount + j].vTranslate =
                        float4(keyFrame.Pos.x, keyFrame.Pos.y, keyFrame.Pos.z, 1.f);

                    vecFrameTrans[i * m_FrameCount + j].qRot = keyFrame.RotQuat;
                }
            }
        }

        if (false == CreateKeyFrameSBuffer(vecFrameTrans))
        {
            return eResult::Fail_InValid;
        }

		return eResult::Success;
	}
    bool Animation3D::CreateKeyFrameSBuffer(const std::vector<tAnimFrameTranslation>& _vecAnimFrameTranslations)
    {
        MH_ASSERT(false == _vecAnimFrameTranslations.empty());

        m_KeyFrameBuffer = std::make_unique<StructBuffer>();
        tSBufferDesc desc{};
        desc.eSBufferType = eStructBufferType::READ_ONLY;
        desc.REGISLOT_t_SRV = Register_t_g_FrameTransArray;
        desc.TargetStageSRV = define::eShaderStageFlag::CS;
        m_KeyFrameBuffer->SetDesc(desc);

        m_KeyFrameBuffer->Create<tAnimFrameTranslation>
            (_vecAnimFrameTranslations.size(), _vecAnimFrameTranslations.data(), _vecAnimFrameTranslations.size());
    }
}
