#include <vector>
#include <unordered_map>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Md2Parts.h"
#include "GlObj.h"

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

std::tuple<bool, GLuint> GlObj::LoadShaders(const std::string &vshstrdata, const std::string &fshstrdata) {
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

void GlObj::DeleteShaders(GLuint programId) {
    glDeleteProgram(programId);
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

/* シェーダのAttribute属性一括設定 */
RetShaderAttribs GlObj::setAttribute(GLuint programId, int totalframes,
                                     const std::vector<vertex> &vertexs, const std::vector<mesh> &polyIndexs,const std::vector<texstcoord> &sts) {
    /* 返却値 */
    std::unordered_map<int, std::pair<int, int>> retAnimFrameS2e;
    GLuint retVbo = -1;
    GLuint retCurPosAttrib   = glGetAttribLocation( programId, "pos");
    GLuint retNextPosAttrib  = glGetAttribLocation( programId, "nextPos");
    GLuint retTexCoordAttrib = glGetAttribLocation( programId, "texCoord");

    /* 初期知設定 */
    std::vector<float> wkMd2Vertices = {0};
    const size_t numPolys           = polyIndexs.size();
    const size_t numVertexsperframe = vertexs.size() / totalframes;

    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "endFrame=%d numPoly=%d numVertexsperframe=%d %s %s(%d)", totalframes, numPolys, numVertexsperframe, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

//    2022-11-02 10:41:10.013 24069-24094/com.tks.cppmd2viewer I/aaaaa: endFrame=198 numPoly=592 numVertexsperframe=312 static RetShaderAttribs GlObj::setAttribute(GLuint, int, const std::vector<vertex> &, const std::vector<mesh> &, const std::vector<texstcoord> &) GlObj.cpp(159)
//    2022-11-02 10:41:10.310 24069-24094/com.tks.cppmd2viewer I/aaaaa: endFrame=198 numPoly=590 numVertexsperframe=315 static RetShaderAttribs GlObj::setAttribute(GLuint, int, const std::vector<vertex> &, const std::vector<mesh> &, const std::vector<texstcoord> &) GlObj.cpp(159)

//    2022-11-01 20:30:26.843 27392-27431/org.raydelto.md2loader I/aaaaa: endFrame=197 numPoly=592 numVertexsperframe=312 void Raydelto::MD2Loader::MD2Model::InitBuffer() MD2Model.cpp(100)
//    2022-11-01 20:30:27.120 27392-27431/org.raydelto.md2loader I/aaaaa: count=1776 (m_frameIndices[frameIndex].first * 8)=0  void Raydelto::MD2Loader::MD2Model::InitBuffer() MD2Model.cpp(144)
//    2022-11-01 20:30:27.170 27392-27431/org.raydelto.md2loader I/aaaaa: endFrame=197 numPoly=590 numVertexsperframe=315 void Raydelto::MD2Loader::MD2Model::InitBuffer() MD2Model.cpp(100)
//    2022-11-01 20:30:27.435 27392-27431/org.raydelto.md2loader I/aaaaa: count=1770 (m_frameIndices[frameIndex].first * 8)=0  void Raydelto::MD2Loader::MD2Model::InitBuffer() MD2Model.cpp(144)

    /* 現在頂点,次頂点,UV座標で詰替え */
    for(int frameidx = 0; frameidx < totalframes; frameidx++) {
        /* 現在フレームと次フレームを取得 */
        const vertex *currentFrame= &vertexs[numVertexsperframe * frameidx];
        const vertex *nextFrame   = (frameidx+1 >= totalframes) ? &vertexs[0] : &vertexs[numVertexsperframe * (frameidx+1)];

        for(int plyidx = 0; plyidx < numPolys; plyidx++) {
            for(int meshdx = 0; meshdx < 3; meshdx++) {
                /* vertices */
                for (size_t vidx = 0; vidx < 3; vidx++) {
                    wkMd2Vertices.emplace_back(currentFrame[polyIndexs[plyidx].meshIndex[meshdx]].v[vidx]);
                }

                /* next frame */
                for (size_t vidx = 0; vidx < 3; vidx++) {
                    // vertices
                    wkMd2Vertices.emplace_back(nextFrame[polyIndexs[plyidx].meshIndex[meshdx]].v[vidx]);
                }

                /* tex coords */
                wkMd2Vertices.emplace_back(sts[polyIndexs[plyidx].stIndex[meshdx]].s);
                wkMd2Vertices.emplace_back(sts[polyIndexs[plyidx].stIndex[meshdx]].t);
            }
        }

        int startverindex= (frameidx==0) ? 0 : retAnimFrameS2e[frameidx-1].second + 1;
        int endverindex  = numPolys * 3 - 1;
        retAnimFrameS2e[frameidx] = {startverindex, endverindex};
    }

    /* TODO 削除予定 */
    for(auto [key, val] : retAnimFrameS2e) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "m_frameIndices[%d]={sv:%d, ev:%d}", key, val.first, val.second);
//        2022-11-01 17:44:07.691 14015-14075/org.raydelto.md2loader I/aaaaa: m_frameIndices[197]={sv:349872, ev:351647}
//        2022-11-01 17:44:07.691 14015-14075/org.raydelto.md2loader I/aaaaa: m_frameIndices[196]={sv:348096, ev:349871}
//        2022-11-01 17:44:07.691 14015-14075/org.raydelto.md2loader I/aaaaa: m_frameIndices[195]={sv:346320, ev:348095}
//        2022-11-01 17:44:07.691 14015-14075/org.raydelto.md2loader I/aaaaa: m_frameIndices[194]={sv:344544, ev:346319}
//        2022-11-01 17:44:07.691 14015-14075/org.raydelto.md2loader I/aaaaa: m_frameIndices[2]={sv:3552, ev:5327}
//        2022-11-01 17:44:07.691 14015-14075/org.raydelto.md2loader I/aaaaa: m_frameIndices[1]={sv:1776, ev:3551}
//        2022-11-01 17:44:07.691 14015-14075/org.raydelto.md2loader I/aaaaa: m_frameIndices[0]={sv:0, ev:1775}
    }

    size_t numVertexs = numPolys * 3 + 1;
    glGenBuffers(1, &retVbo);

    glBindBuffer(GL_ARRAY_BUFFER, retVbo);
    glBufferData(GL_ARRAY_BUFFER, numVertexs * sizeof(float) * 8 * totalframes, &wkMd2Vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(retCurPosAttrib  , 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)nullptr);
    glVertexAttribPointer(retNextPosAttrib , 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(retTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

    glEnableVertexAttribArray(retCurPosAttrib);
    glEnableVertexAttribArray(retNextPosAttrib);
    glEnableVertexAttribArray(retTexCoordAttrib);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return {true, retAnimFrameS2e, retVbo, retCurPosAttrib, retNextPosAttrib, retTexCoordAttrib};
}
