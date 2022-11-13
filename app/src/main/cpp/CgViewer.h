#ifndef CPPMD2VIEWER_CGVIEWER_H
#define CPPMD2VIEWER_CGVIEWER_H

#include <map>
#include <string>
#include "Md2Model.h"

class CgViewer {
public:
    static bool LoadModel(std::map<std::string, TmpBinData1> &tmpbindata1);
    static bool InitModel(const std::map<std::string, TmpBinData3> &tmpbindata3s);
    static void setViewerArea(int width, int height);
    static bool DrawModel(float elapsedtimeMs);
};

#endif //CPPMD2VIEWER_CGVIEWER_H
