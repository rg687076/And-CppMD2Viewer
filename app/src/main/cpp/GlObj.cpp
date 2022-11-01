#include "GlObj.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

/* OpenGLのTexture初期化 */
std::tuple<bool, GLuint>  GlObj::TexInit(int width, int height, const char *rgbabindbuf) {
    bool generateMipMaps = true;    /* ひとまず、true固定 */

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

std::tuple<bool, GLuint>  GlObj::LoadShaders(const std::string &vshstrdata, const std::string &fshstrdata) {
    const GLchar *vsSourcePtr = vshstrdata.c_str();
    const GLchar *fsSourcePtr = fshstrdata.c_str();

    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vsid, 1, &vsSourcePtr, nullptr);
    glShaderSource(fsid, 1, &fsSourcePtr, nullptr);

    glCompileShader(vsid);
    bool ret0 = CheckCompileErrors(vsid, EShaderType::VERTEX_SHADER);
    if( !ret0) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to compile vertex shader.");
        glDeleteShader(vsid);
        return {false, -1};
    }

    glCompileShader(fsid);
    bool ret1 = CheckCompileErrors(fsid, EShaderType::FRAGMENT_SHADER);
    if( !ret1) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to compile fragment shader.");
        glDeleteShader(vsid);
        glDeleteShader(fsid);
        return {false, -1};
    }

    GLuint retProgramId = glCreateProgram();
    if (retProgramId == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to create shader program!");
        return {false, -1};
    }

    /* Attach shader to program. */
    glAttachShader(retProgramId, vsid);
    glAttachShader(retProgramId, fsid);

    glLinkProgram(retProgramId);
    bool ret2 = CheckLinkError(retProgramId);
    if ( !ret2) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed!! Shader program failed to link.");
        glDetachShader(retProgramId, vsid);
        glDetachShader(retProgramId, fsid);
        glDeleteShader(vsid);
        glDeleteShader(fsid);
        glDeleteProgram(retProgramId);
        return {false, -1};
    }

    /* 準備完了 */
    glDeleteShader(vsid);
    glDeleteShader(fsid);

    return {true, retProgramId};
}

/* コンパイル結果判定 */
bool GlObj::CheckCompileErrors(GLuint id, EShaderType type) {
    int isCompiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

    /* 正常終了 */
    if (isCompiled != GL_FALSE)
        return true;

    /* 失敗 エラー文字列長取得 */
    int maxLength = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
    /* 格納領域確保 */
    char *gllogstr = (char*)malloc(maxLength);
    /* 結果文字列取得 */
    glGetShaderInfoLog(id, maxLength, &maxLength, gllogstr);
    if (type == EShaderType::VERTEX_SHADER) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "VertexShader : %s\n", gllogstr);
    }
    else {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "FragmentShader : %s\n", gllogstr);
    }
    /* 格納領域解放 */
    free(gllogstr);

    return false;
}

bool GlObj::CheckLinkError(GLuint programId) {
    int status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if(status != 0)
        return true;

    int logLength;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char *logstr = (char*)malloc(logLength);
        glGetProgramInfoLog(programId, logLength, &logLength, logstr);
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Program link Error!! : \n%s", logstr);
        free(logstr);
    }

    return false;
}
