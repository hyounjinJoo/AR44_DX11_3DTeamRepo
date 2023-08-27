#pragma once
#include "Entity.h"
#include "SimpleMath.h"
#include "Shader_Common.h"

namespace mh
{
    struct tKeyFrame
    {
        double  Time{};
        float3  Scale{};
        float4  RotQuat{};
        float3  Pos{};
    };

    struct tKeyFramesPerBone
    {
        int                         BoneIndex{};
        std::vector<tKeyFrame>     vecKeyFrame{};
    };


    struct tFBXAnimClip;
    class StructBuffer;

	class Animation3D :
		public Entity
	{
	public:
		Animation3D();
        virtual ~Animation3D();

    public:
        void BindData();
        void UnBindData();


        eResult LoadFromFBX(const tFBXAnimClip* _clip);
        bool IsSequenceEnd() const { return m_End; }
        
        const std::vector<tKeyFramesPerBone>& GetKeyFrames() { return mKeyFramesPerBone; }
        const std::string& GetAnimationName() const { return mAnimationName; }


    private:
        bool CreateKeyFrameBuffer(const std::vector<tAnimFrameTranslation>& _vecAnimFrameTranslations);

    private:
        std::string                 mAnimationName;
        float						m_StartTime;
        float						m_EndTime;
        float						m_TimeLength;
        float						m_FrameTime;
        float						m_PlayTime;
        float						m_PlayScale;
        int							m_StartFrame;
        int							m_EndFrame;
        int							m_FrameCount;   //전체 프레임 카운트 수
        int							m_FrameMode;
        int							m_ChangeFrame;
        bool						m_End;

        //시스템 메모리 저장용 키프레임 데이터
        //순서 = 본 번호, 내부에는 각 본 별 키프레임 데이터가 저장되어 있음.
        std::vector<tKeyFramesPerBone>	mKeyFramesPerBone;
        std::unique_ptr<StructBuffer> m_KeyFrameBuffer;
	};
}


