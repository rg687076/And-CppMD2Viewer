#ifndef CPPMD2VIEWER_GLOBJ_H
#define CPPMD2VIEWER_GLOBJ_H

#include <map>
#include <string>

class ShaderInfo {
public:
    std::string name;
    std::string filename;
    std::string content;
};

/* Md2モデルs */
extern std::map<std::string, ShaderInfo> gShaderInfo;
/* OpenGL初期化 */
extern void GlInit(std::map<std::string, ShaderInfo> &map);
#endif //CPPMD2VIEWER_GLOBJ_H
