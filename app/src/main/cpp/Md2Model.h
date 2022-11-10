#ifndef CPPMD2VIEWER_MD2MODEL_H
#define CPPMD2VIEWER_MD2MODEL_H

#include <string>
#include <vector>

class TmpBinData {
public:
    std::string         mName = {};
    std::vector<char>   mWkMd2BinData = {};
    std::vector<char>   mWkTexBinData = {};
    std::string         mWkVshStrData = {};
    std::string         mWkFshStrData = {};
    int                 mWkWidth = 0;
    int                 mWkHeight= 0;
    std::vector<char>   mWkRgbaData = {};
};

#endif //CPPMD2VIEWER_MD2MODEL_H
