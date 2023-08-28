#pragma once
#include "Entity.h"
#include "SimpleMath.h"
#include "Shader_Common.h"

namespace mh
{
    struct tKeyFrame
    {
        double              Time{};
        float3              Scale{};
        math::Quaternion    RotQuat{};
        float3              Pos{};
    };

    struct tKeyFramesPerBone
    {
        int                         BoneIndex{};
        std::vector<tKeyFrame>     vecKeyFrame{};
    };


    struct tFBXAnimClip;
    class StructBuffer;
    class Skeleton;
	class Animation3D :
		public Entity
	{
	public:
		Animation3D();

        Animation3D(const Animation3D& _other) = delete;
        Animation3D(Animation3D&& _move);

        virtual ~Animation3D();

    public:
        void BindData();
        void UnBindData();

        eResult LoadFromFBX(Skeleton* _skeleton, const tFBXAnimClip* _clip);

        int GetStartFrame() const { return mValues.iStartFrame; }
        int GetEndFrame() const { return mValues.iEndFrame; }
        int GetFrameLength() const { return mValues.iFrameLength; }
        double GetStartTime() const { return mValues.dStartTime; }
        double GetEndTIme() const { return mValues.dEndTime; }
        double GetTimeLength() const { return mValues.dTimeLength; }
        float GetUpdateTime() const { return mValues.fUpdateTime; }
        int GetFPS() const { return mValues.iFramePerSec; }
        std::weak_ptr<Skeleton> GetSkeleton() const { return m_OwnerSkeleton; }

    private:
        bool CreateKeyFrameSBuffer(const std::vector<tAnimKeyframeTranslation>& _vecAnimFrameTranslations);

    private:
        struct Value//저장을 위해서 별도의 struct 안에 넣어놓음
        {
            int				iStartFrame;
            int				iEndFrame;
            int				iFrameLength;

            double			dStartTime;
            double			dEndTime;
            double			dTimeLength;
            float			fUpdateTime; // 이거 안씀

            int         	iFramePerSec;
        } mValues;

        std::weak_ptr<Skeleton>                 m_OwnerSkeleton;

        //이중 배열 형태임
        std::vector<tKeyFramesPerBone>          m_KeyFrames;
        std::unique_ptr<StructBuffer>			m_SBufferKeyFrame;
	};
}


