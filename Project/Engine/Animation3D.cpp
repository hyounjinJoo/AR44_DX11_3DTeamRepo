#include "PCH_Engine.h"
#include "Animation3D.h"
#include "FBXLoader.h"
#include "StructBuffer.h"
#include "Skeleton.h"
#include "PathMgr.h"
#include "define_Util.h"

namespace mh
{
	Animation3D::Animation3D()
        : mValues{}
        , m_OwnerSkeleton{}
        , m_KeyFramesPerBone{}
        , m_SBufferKeyFrame{}
	{
	}

    Animation3D::~Animation3D()
	{
	}
    void Animation3D::BindData()
    {
        MH_ASSERT(m_SBufferKeyFrame);
        m_SBufferKeyFrame->BindDataSRV();
    }
    void Animation3D::UnBindData()
    {
        m_SBufferKeyFrame->UnBindData();
    }
    eResult Animation3D::Save(const std::filesystem::path& _filePath)
    {
        

        std::fs::path filePath = PathMgr::GetContentPathRelative(eResourceType::MeshData);

        



        return eResult();
    }
    eResult Animation3D::Load(const std::filesystem::path& _filePath)
    {
        return eResult();
    }
	eResult Animation3D::LoadFromFBX(Skeleton* _skeleton, const tFBXAnimClip* _clip)
	{
		if (nullptr == _skeleton || nullptr == _clip)
			return eResult::Fail_Nullptr;

        m_OwnerSkeleton = _skeleton;

        SetKey(_clip->strName);

        switch (_clip->TimeMode)
        {
        case fbxsdk::FbxTime::eDefaultMode:
            mValues.iFramePerSec = 30;
            break;
        case fbxsdk::FbxTime::eFrames120:
            mValues.iFramePerSec = 120;
            break;
        case fbxsdk::FbxTime::eFrames100:
            mValues.iFramePerSec = 100;
            break;
        case fbxsdk::FbxTime::eFrames60:
            mValues.iFramePerSec = 60;
            break;
        case fbxsdk::FbxTime::eFrames50:
            mValues.iFramePerSec = 50;
            break;
        case fbxsdk::FbxTime::eFrames48:
            mValues.iFramePerSec = 48;
            break;
        case fbxsdk::FbxTime::eFrames30:
            [[fallthrough]];
        case fbxsdk::FbxTime::eFrames30Drop:
            [[fallthrough]];
        case fbxsdk::FbxTime::eNTSCDropFrame:
            [[fallthrough]];
        case fbxsdk::FbxTime::eNTSCFullFrame:
            mValues.iFramePerSec = 30;
            break;
        case fbxsdk::FbxTime::ePAL:
            mValues.iFramePerSec = 25;
            break;
        case fbxsdk::FbxTime::eFrames24:
            mValues.iFramePerSec = 24;
            break;
        case fbxsdk::FbxTime::eFrames1000:
            mValues.iFramePerSec = 1000;
            break;
        case fbxsdk::FbxTime::eCustom:
            mValues.iFramePerSec = 30;
            break;
        case fbxsdk::FbxTime::eFrames96:
            mValues.iFramePerSec = 96;
            break;
        case fbxsdk::FbxTime::eFrames72:
            mValues.iFramePerSec = 72;
            break;
        default:
            mValues.iFramePerSec = 60;
            break;
        }

        mValues.dStartTime = _clip->StartTime.GetSecondDouble();
        mValues.dEndTime = _clip->EndTime.GetSecondDouble();
        mValues.dTimeLength = mValues.dEndTime - mValues.dStartTime;

        mValues.iStartFrame = (int)_clip->StartTime.GetFrameCount(_clip->TimeMode);
        mValues.iEndFrame = (int)_clip->EndTime.GetFrameCount(_clip->TimeMode);
        mValues.iFrameLength = (int)(mValues.iEndFrame - mValues.iStartFrame + 1);//+1 -> 0프레임부터 시작이므로
        MH_ASSERT(mValues.iFrameLength >= 0);


        //본의 사이즈 * 프레임 수 만큼 사이즈를 설정
        //그러므로 CPU 메모리에서는 이중 배열 형태로 저장해 놓고
        // 본 3개 키프레임 5개일 경우
        // 본0키0|본0키1|본0키2|본0키3|본0키4|
        // 본1키0|본1키1|본1키2|본1키3|본1키4|
        // 본2키0|본2키1|본2키2|본2키3|본2키4
        //그러므로 하나의 애니메이션은 본의 갯수 * 키프레임 갯수가 된다
        std::vector<tAnimKeyframeTranslation>	vecFrameTrans;//GPU
        size_t boneCount = _clip->KeyFramesPerBone.size();
        m_KeyFramesPerBone.resize(boneCount);//CPU
        for (size_t i = 0; i < boneCount; ++i)
        {
            m_KeyFramesPerBone[i].BoneIndex = _clip->KeyFramesPerBone[i].BoneIndex;
            //m_KeyFramesPerBone[i].vecKeyFrame.resize(_clip->KeyFramesPerBone[i].vecKeyFrame.size());
            m_KeyFramesPerBone[i].vecKeyFrame.resize(mValues.iFrameLength);

            //GPU로 보낼 데이터 세팅
            //GPU는 이중 배열 같은걸 지원 안함
            //GPU 메모리는 이걸 1차원 배열 형태로 펴서 데이터를 보내준다
            //그러니까 1차원 배열 형태로 애니메이션 데이터를 보내야 함
            
            //FBX가 왜인지는 모르겠지만 키프레임이 비정상적으로 로딩되는 경우가 있어서 
            //키프레임 갯수는 고정된 프레임 수로 생성해준다
            //본의 사이즈 * 프레임 수(2차 배열 -> 1차 배열)
            
            vecFrameTrans.resize(boneCount * mValues.iFrameLength);
            for(size_t j = 0; j < mValues.iFrameLength; ++j)
            {
                //우리 포맷 키프레임
                tKeyFrame& projKeyFrame = m_KeyFramesPerBone[i].vecKeyFrame[j];
                //FBX 포맷 키프레임
                const tFBXKeyFrame& fbxKeyFrame = _clip->KeyFramesPerBone[i].vecKeyFrame[j];

                //포맷을 변환해줘야 한다. double -> float

                const fbxsdk::FbxVector4& fbxPos = fbxKeyFrame.matTransform.GetT();
                projKeyFrame.Pos.x = (float)fbxPos.mData[0];
                projKeyFrame.Pos.y = (float)fbxPos.mData[1];
                projKeyFrame.Pos.z = (float)fbxPos.mData[2];

                const fbxsdk::FbxVector4& fbxScale = fbxKeyFrame.matTransform.GetS();
                projKeyFrame.Scale.x = (float)fbxScale.mData[0];
                projKeyFrame.Scale.y = (float)fbxScale.mData[1];
                projKeyFrame.Scale.z = (float)fbxScale.mData[2];

                const fbxsdk::FbxQuaternion& fbxQuat = fbxKeyFrame.matTransform.GetQ();
                projKeyFrame.RotQuat.x = (float)fbxQuat.mData[0];
                projKeyFrame.RotQuat.y = (float)fbxQuat.mData[1];
                projKeyFrame.RotQuat.z = (float)fbxQuat.mData[2];
                projKeyFrame.RotQuat.w = (float)fbxQuat.mData[3];

                

                //i번째 본의 j 키프레임 데이터
                size_t indexIn1DArray = mValues.iFrameLength * i + j;
                vecFrameTrans[indexIn1DArray].vTranslate =  float4(projKeyFrame.Pos, 1.f);
                vecFrameTrans[indexIn1DArray].vScale =      float4(projKeyFrame.Scale, 1.f);
                vecFrameTrans[indexIn1DArray].qRot =        projKeyFrame.RotQuat;
            }
        }

        if (false == CreateKeyFrameSBuffer(vecFrameTrans))
        {
            return eResult::Fail_InValid;
        }

		return eResult::Success;
	}
    bool Animation3D::CreateKeyFrameSBuffer(const std::vector<tAnimKeyframeTranslation>& _vecAnimFrameTranslations)
    {
        MH_ASSERT(false == _vecAnimFrameTranslations.empty());

        m_SBufferKeyFrame = std::make_unique<StructBuffer>();
        tSBufferDesc desc{};
        desc.eSBufferType = eStructBufferType::READ_ONLY;
        desc.REGISLOT_t_SRV = Register_t_g_FrameTransArray;
        desc.TargetStageSRV = define::eShaderStageFlag::CS;
        m_SBufferKeyFrame->SetDesc(desc);

        if (FAILED(m_SBufferKeyFrame->Create<tAnimKeyframeTranslation>
            (_vecAnimFrameTranslations.size(), _vecAnimFrameTranslations.data(), _vecAnimFrameTranslations.size())))
        {
            return false;
        }

        return true;
    }
}
