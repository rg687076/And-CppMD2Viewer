#ifndef CPPMD2VIEWER_GLOBALSPACEOBJ_H
#define CPPMD2VIEWER_GLOBALSPACEOBJ_H

#include <array>
#include "MatVec.h"

class GlobalSpacePrm {
public:
    std::array<float, 16> mProjectionMat= MatVec::IDENTITY;
    std::array<float, 16> mViewMat      = MatVec::IDENTITY;
    std::array<float, 16> mNormalMat    = MatVec::IDENTITY;
    std::array<float,  3> mCameraPos = {};
    std::array<float,  3> mTargetPos = {};
    std::array<float,  3> mUpPos     = {};
};

#endif //CPPMD2VIEWER_GLOBALSPACEOBJ_H