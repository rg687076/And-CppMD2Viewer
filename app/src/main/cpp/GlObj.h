#ifndef CPPMD2VIEWER_GLOBJ_H
#define CPPMD2VIEWER_GLOBJ_H

#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Md2Parts.h"

enum class EShaderType : int {
    VERTEX_SHADER = 0,
    FRAGMENT_SHADER,
};

using RetShaderAttribs = std::tuple<bool, std::unordered_map<int, std::pair<int, int>>, GLuint, GLuint, GLuint, GLuint>;

class GlObj {
public:
    /* OpenGL初期化 */
    static void GlInit();
    static void enable(GLenum cap);
    /* Texture初期化 */
    static std::tuple<bool, GLuint> InitTexture(int width, int height, const char *rgbabindbuf);
    static std::tuple<bool, GLuint> LoadShaders(const std::string &vshstrdata, const std::string &fshstrdata);
    static RetShaderAttribs setAttribute(GLuint programId, int totalframes, const std::vector<vertex> &vertexs, const std::vector<mesh> &polyIndexs, const std::vector<texstcoord> &sts);
    static void DeleteShaders(GLuint programId);
    static void setViewport(int x, int y, int width, int height);
    static void clear(GLbitfield mask);
    static void activeTexture(GLenum texture);
    static void bindTexture(GLenum target, GLuint textureid);
    static void deleteTextures(GLsizei size, const GLuint *textures);
    static void useProgram(GLuint programId);
    static void deleteProgram(GLuint progid);
    static void setUniform(GLuint programId, const GLchar *name, const std::array<float, 16> &mat44);
    static void setUniform(GLuint programId, const GLchar *name, GLfloat valf);
    static void bindBuffer(GLenum target, GLuint buffer);
    static void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    static void drawArrays(GLenum mode, GLint first, GLsizei count);

private:
    static bool CheckCompileErrors(GLuint sid, EShaderType type);
    static bool CheckLinkError(GLuint programId);
    static std::map<std::string, GLint>     mUniformLocations;
    static GLint getUniformId(GLuint programId, const GLchar *name);
};

#endif //CPPMD2VIEWER_GLOBJ_H
