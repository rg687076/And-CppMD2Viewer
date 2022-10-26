#ifndef CPPMD2VIEWER_ASSETSDATA_H
#define CPPMD2VIEWER_ASSETSDATA_H

#include <map>
#include <string>
#include <vector>

class AssetsData {
public:
    std::map<std::string, std::vector<char>> mAssets;
    static AssetsData &GetIns();
private:
    AssetsData() {}
    AssetsData(const AssetsData &) {}
    AssetsData &operator=(const AssetsData&){ return *this; }
};
#endif //CPPMD2VIEWER_ASSETSDATA_H
