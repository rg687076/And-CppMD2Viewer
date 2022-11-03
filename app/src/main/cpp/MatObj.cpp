#include <functional>
#include <array>
#include "MatObj.h"

const std::array<float, 16> Mat44::IDENTITY = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
};

std::array<float, 16> Mat44::getPerspectivef(float fovy, float aspect, float near, float far) {
    std::array<float, 16> retMat = {};

    float n2 = near + near;
    float bottom= (float)tan(fovy / 2.0f * PI / 180.0) * near;
    float top   = -bottom;
    float right = aspect * bottom;
    float left  = -right;
    float xx = 1.0f / (left - right);
    float yy = 1.0f / (bottom - top);
    float zz = 1.0f / (near - far);

#define M(row, col) retMat [col * 4 + row]

    M(0, 0) = n2 * xx;
    M(0, 1) = 0.0f;
    M(0, 2) = (right + left) * xx;
    M(0, 3) = 0.0f;

    M(1, 0) = 0.0f;
    M(1, 1) = n2 * yy;
    M(1, 2) = (top + bottom) * yy;
    M(1, 3) = 0.0f;

    M(2, 0) = 0.0f;
    M(2, 1) = 0.0f;
    M(2, 2) = -(far + near) * zz;
    M(2, 3) = -(n2 * far) * zz;

    M(3, 0) = 0.0f;
    M(3, 1) = 0.0f;
    M(3, 2) = -1.0f;
    M(3, 3) = 0.0f;

#undef M

    return retMat;
}

/* 行列の掛け算(外積とは違う) */
std::array<float, 16> Mat44::multMatrixf(const std::array<float, 16> &a, const std::array<float, 16> &m) {
    std::array<float, 16> retmat = {};

#define A(row, col) a[(col << 2) + row]
#define M(row, col) m[(col << 2) + row]
#define MAT(row, col) retmat[(col << 2) + row]

    for (int i = 0; i < 4; i++) {
        MAT(i, 0) = A(i, 0) * M(0, 0) + A(i, 1) * M(1, 0) + A(i, 2) * M(2, 0) + A(i, 3) * M(3, 0);
        MAT(i, 1) = A(i, 0) * M(0, 1) + A(i, 1) * M(1, 1) + A(i, 2) * M(2, 1) + A(i, 3) * M(3, 1);
        MAT(i, 2) = A(i, 0) * M(0, 2) + A(i, 1) * M(1, 2) + A(i, 2) * M(2, 2) + A(i, 3) * M(3, 2);
        MAT(i, 3) = A(i, 0) * M(0, 3) + A(i, 1) * M(1, 3) + A(i, 2) * M(2, 3) + A(i, 3) * M(3, 3);
    }

#undef A
#undef M
#undef MAT
    return retmat;
}

/* setLookAtf */
std::array<float, 16> Mat44::getLookAtf(const std::array<float, 3> &camerapos,
                                        const std::array<float, 3> &targetpos,
                                        const std::array<float, 3> &uppos) {
    std::array<float, 16> retMat = {};
    std::array<float, 3> view0 = {targetpos[0]-camerapos[0],
                                 targetpos[1]-camerapos[1],
                                 targetpos[2]-camerapos[2]};
    std::array<float, 3> up0 = uppos;

    std::array<float, 3> view1= Vec3::normalize(view0);
    std::array<float, 3> up1  = Vec3::normalize(up0);

    std::array<float, 3> side0 = Vec3::cross(view1, up1);
    std::array<float, 3> side1 = Vec3::normalize(side0);

    std::array<float, 3> up2 = Vec3::cross(side1, view1);

#define M(row, col) retMat[(col << 2) + row]

    M(0, 0) = side1[0];
    M(0, 1) = side1[1];
    M(0, 2) = side1[2];
    M(0, 3) = 0.0f;

    M(1, 0) = up2[0];
    M(1, 1) = up2[1];
    M(1, 2) = up2[2];
    M(1, 3) = 0.0f;

    M(2, 0) = -view1[0];
    M(2, 1) = -view1[1];
    M(2, 2) = -view1[2];
    M(2, 3) = 0.0f;

    M(3, 0) = 0.0f;
    M(3, 1) = 0.0f;
    M(3, 2) = 0.0f;
    M(3, 3) = 1.0f;

#undef M

    retMat = Mat44::translatef(retMat, {-camerapos[0], -camerapos[1], -camerapos[2]});

    return retMat;
}

/* 移動行列を設定 */
std::array<float, 16> Mat44::translatef(const std::array<float, 16> &mat, const std::array<float, 3> &vec) {
    std::array<float, 16> retmat = mat;
    retmat[12] = mat[0] * vec[0] + mat[4] * vec[1] + mat[ 8] * vec[2] + mat[12];
    retmat[13] = mat[1] * vec[0] + mat[5] * vec[1] + mat[ 9] * vec[2] + mat[13];
    retmat[14] = mat[2] * vec[0] + mat[6] * vec[1] + mat[10] * vec[2] + mat[14];
    retmat[15] = mat[3] * vec[0] + mat[7] * vec[1] + mat[11] * vec[2] + mat[15];
    return retmat;
}

/* 正規化 */
std::array<float, 3> Vec3::normalize(const std::array<float, 3> &vec) {
    std::array<float, 3> retvec = vec;

    float l = (float)sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    if (l == 0.0f)
        return retvec;

    retvec[0] /= l;
    retvec[1] /= l;
    retvec[2] /= l;

    return retvec;
}

/* 外積(ベクトルのみ) */
std::array<float, 3> Vec3::cross(const std::array<float, 3> &v1, const std::array<float, 3> &v2) {
    std::array<float, 3> retvec = {};
    retvec[0] = v1[1] * v2[2] - v1[2] * v2[1];
    retvec[1] = v1[2] * v2[0] - v1[0] * v2[2];
    retvec[2] = v1[0] * v2[1] - v1[1] * v2[0];
    return retvec;
}
