#ifndef CPPMD2VIEWER_CGVIEWER_H
#define CPPMD2VIEWER_CGVIEWER_H

#include <map>
#include <string>
#include "Md2Model.h"

class CgViewer {
public:
    static bool LoadModel(std::map<std::string, TmpBinData1> &tmpbindata1, const std::map<std::string, std::tuple<float, float, float, float, float, float, float, float, float>> &initPosition);
    static bool InitModel(const std::map<std::string, TmpBinData3> &tmpbindata3s);
    static void SetViewerArea(int width, int height);
    static bool DrawModel(float elapsedtimeMs);
    static void SetVpMat(const std::array<float, 16> &vpmat);
    static void SetRotate(float rotatex, float rotatey);
    static void SetScale(float scale);
    static void SetPosition(const std::string &key, const std::array<float, 3> &pos);
};

#endif //CPPMD2VIEWER_CGVIEWER_H
