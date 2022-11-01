#ifndef CPPMD2VIEWER_GLOBJ_H
#define CPPMD2VIEWER_GLOBJ_H

#include <map>
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

enum class EShaderType : int {
    VERTEX_SHADER = 0,
    FRAGMENT_SHADER,
};

class GlObj {
public:
    /* Texture初期化 */
    static std::tuple<bool, GLuint> TexInit(int width, int height, const char *rgbabindbuf);
    static std::tuple<bool, GLuint> LoadShaders(const std::string &vshstrdata, const std::string &fshstrdata);

private:
    static bool CheckCompileErrors(GLuint sid, EShaderType type);
    static bool CheckLinkError(GLuint programId);
};
#endif //CPPMD2VIEWER_GLOBJ_H
