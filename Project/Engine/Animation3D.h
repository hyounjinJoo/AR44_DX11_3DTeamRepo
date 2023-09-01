#pragma once
#include "Entity.h"
#include "SimpleMath.h"
#include "Shader_Common.h"

namespace mh
{
    struct tKeyFrame
    {
        double              Time{};
        tAnimKeyframeTranslation Trans{};
        //float3              Scale{};
        //math::Quaternion    RotQuat{};
        //float3              Pos{};
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
        friend class Skeleton;
	public:
		Animation3D();

        Animation3D(const Animation3D& _other) = delete;

        virtual ~Animation3D();

    public:
        void BindData();
        void UnBindData();

        eResult Save(const std::fs::path& _filePath, const std::fs::path& _basePath);
        eResult Load(const std::fs::path& _filePath, const std::fs::path& _basePath);

        eResult LoadFromFBX(Skeleton* _skeleton, const tFBXAnimClip* _clip);
        

        int GetStartFrame() const { return mValues.iStartFrame; }
        int GetEndFrame() const { return mValues.iEndFrame; }
        int GetFrameLength() const { return mValues.iFrameLength; }
        double GetStartTime() const { return mValues.dStartTime; }
        double GetEndTIme() const { return mValues.dEndTime; }
        double GetTimeLength() const { return mValues.dTimeLength; }
        float GetUpdateTime() const { return mValues.fUpdateTime; }
        int GetFPS() const { return mValues.iFramePerSec; }
        Skeleton* GetSkeleton() const { return m_OwnerSkeleton; }
        void SetSkeleton(Skeleton* _skeleton) { m_OwnerSkeleton = _skeleton; }
        StructBuffer* GetKeyFrameSBuffer() const { return m_SBufferKeyFrame.get(); }

    private:
        bool CreateKeyFrameSBuffer(const std::vector<tAnimKeyframeTranslation>& _vecAnimFrameTranslations);

    private:
        Skeleton* m_OwnerSkeleton;

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


        //이중 배열 형태임
        std::vector<tKeyFramesPerBone>          m_KeyFramesPerBone;
        std::unique_ptr<StructBuffer>			m_SBufferKeyFrame;
	};
}


