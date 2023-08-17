#pragma once
#include "IComponent.h"

namespace mh
{
    class ILight :
        public IComponent
    {
    public:
        ILight(define::eDimensionType _dimensionType);
        virtual ~ILight();

        virtual void Render() = 0;

        define::eDimensionType GetDimensionType() const { return mDimension; }

    private:
        define::eDimensionType mDimension;
    };
}

