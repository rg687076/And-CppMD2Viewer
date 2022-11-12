#ifndef CPPMD2VIEWER_CGVIEWER_H
#define CPPMD2VIEWER_CGVIEWER_H

#include <map>
#include <string>
#include "Md2Model.h"

class CgViewer {
public:
    static bool LoadModel(std::map<std::string, TmpBinData1> &tmpbindata1);
//    static bool InitModel(std::map<std::string, Md2Model> &md2models);
//    static bool DrawModel(std::map<std::string, Md2Model> &md2models, const std::array<float, 16> &amNormalMat, float elapsedtimeMs);
//    static void SetScale(std::map<std::string, Md2Model> &md2models, float scale);
//    static void SetRotate(std::map<std::string, Md2Model> &md2models, float x, float y);
};

#endif //CPPMD2VIEWER_CGVIEWER_H
