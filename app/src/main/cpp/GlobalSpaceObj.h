#ifndef CPPMD2VIEWER_GLOBALSPACEOBJ_H
#define CPPMD2VIEWER_GLOBALSPACEOBJ_H

#include <array>
#include "MatVec.h"

class GlobalSpaceObj {
public:
    std::array<float, 16> mProjectionMat= Mat44::IDENTITY;
    std::array<float, 16> mViewMat      = Mat44::IDENTITY;
    std::array<float, 16> mVpMat        = Mat44::IDENTITY;
    std::array<float, 16> mNormalMatrix = Mat44::IDENTITY;
    std::array<float,  3> mCameraPos = {};
    std::array<float,  3> mTargetPos = {};
    std::array<float,  3> mUpPos     = {};
};

#endif //CPPMD2VIEWER_GLOBALSPACEOBJ_H