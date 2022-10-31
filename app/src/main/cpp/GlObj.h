#ifndef CPPMD2VIEWER_GLOBJ_H
#define CPPMD2VIEWER_GLOBJ_H

#include <map>
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class ShaderInfo {
public:
    std::string name;
    std::string filename;
    std::string content;
};

class GlObj {
public:
    /* Texture初期化 */
    static std::tuple<bool, GLuint> TexInit(int width, int height, const char *rgbabindbuf);
};

/* Md2モデルs */
extern std::map<std::string, ShaderInfo> gShaderInfo;
#endif //CPPMD2VIEWER_GLOBJ_H
