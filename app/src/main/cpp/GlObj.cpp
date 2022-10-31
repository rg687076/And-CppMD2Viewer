#include "GlObj.h"

std::map<std::string, ShaderInfo> gShaderInfo;

/* OpenGLのTexture初期化 */
std::tuple<bool, GLuint>  GlObj::TexInit(int width, int height, const char *rgbabindbuf) {
    bool generateMipMaps = true;    /* ひとまず、true固定値 */

    GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (generateMipMaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbabindbuf);

    if (generateMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    /* 後片付け(unbinding our texture) */
    glBindTexture(GL_TEXTURE_2D, 0);

    return {true, texid};
}
