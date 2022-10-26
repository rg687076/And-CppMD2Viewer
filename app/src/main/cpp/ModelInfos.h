#ifndef CPPMD2VIEWER_MODELINFO_H
#define CPPMD2VIEWER_MODELINFO_H

#include <map>
#include <string>
#include <vector>

class ModelInfo {
public:
    std::string         name;
    std::string         verfilename;
    std::string         texfilename;
    std::vector<char>   verbindata;
    std::vector<char>   texbindata;
};
#endif //CPPMD2VIEWER_MODELINFO_H
