#pragma warning(disable : 4819)
//
// Created by jun on 2021/06/09.
//
#include <cassert>
#ifdef __ANDROID__
#include <GLES3/gl32.h>
#include <android/log.h>
#else /*__ANDROID__*/
#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/include/GL/glext.h"
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/include/GL/wglext.h"
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/include/GL2/gl2.h"
#include "../CG3DCom.h"

//#pragma comment(lib, "../../../../../../WinCG3DVewer/WinCG3DVewer/lib/libGLESv2.lib")
//#pragma comment(lib, "../../../../../../WinCG3DVewer/WinCG3DVewer/lib/libimage.lib")
#pragma comment(lib, "lib/libGLESv2.lib")
#pragma comment(lib, "lib/libimage.lib")
#pragma comment(lib, "OpenGL32.Lib")
#endif /*__ANDROID__*/
#include "GLES2.h"

bool GLES2::OpenGLInit() {
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","GL_VERSION                  : %s\n", glGetString(GL_VERSION));
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","GL_VENDOR                   : %s\n", glGetString(GL_VENDOR));
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","GL_RENDERER                 : %s\n", glGetString(GL_RENDERER));
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","GL_SHADING_LANGUAGE_VERSION : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","GL_EXTENSIONS               : %s\n", glGetString(GL_EXTENSIONS));

    int value;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","gl_MaxVertexAttribs             (%4d) : %4d\n", 16, value);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &value);
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","gl_MaxVertexUniformVectors      (%4d) : %4d\n", 0, value);
    glGetIntegerv(GL_MAX_VARYING_VECTORS, &value);
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","gl_MaxVaryingVectors            (%4d) : %4d\n", 0, value);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &value);
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","gl_MaxCombinedTextureImageUnits (%4d) : %4d\n", 2, value);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &value);
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","gl_MaxVertexTextureImageUnits   (%4d) : %4d\n", 0, value);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","gl_MaxTextureImageUnits         (%4d) : %4d\n", 2, value);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value);
    __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","gl_MaxFragmentUniformVectors    (%4d) : %4d\n", 0, value);

    glClearColor(0, 0, 0, 0.0f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool GLES2::ShaderInit(const std::vector<char> &colvsh, const std::vector<char> &colfsh,
                       const std::vector<char> &texvsh, const std::vector<char> &texfsh, GlShaderObj aShaders[2], unsigned int &TexId) {
    glGenTextures(1, &TexId);
    // テクスチャユニット0を有効にする(マルチテクスチャする時に設定する。テクスチャユニット0だけはデフォルトで有効になっているので,この処理は意味ない)
    glActiveTexture(GL_TEXTURE0);
    // テクスチャオブジェクトをバインドする
    glBindTexture(GL_TEXTURE_2D, TexId);
    // テクスチャパラメータを設定する
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* カラー用 */
    aShaders[0].programId = (int)GLES2::loadShaders(colvsh, colfsh);
    if(aShaders[0].programId == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to loadShaders() id=%d line=%d\n", aShaders[0].programId, __LINE__);
        return false;
    }
    aShaders[0].a_VertexId          = glGetAttribLocation( aShaders[0].programId, "a_Vertex");
    aShaders[0].a_NormalId          = glGetAttribLocation( aShaders[0].programId, "a_Normal");
    aShaders[0].a_ColorId           = glGetAttribLocation( aShaders[0].programId, "a_Color");
    aShaders[0].a_TexCoordId        = -1;
    aShaders[0].u_MvpMatrixId       = glGetUniformLocation(aShaders[0].programId, "u_MvpMatrix");
    aShaders[0].u_NormalMatrixId    = glGetUniformLocation(aShaders[0].programId, "u_NormalMatrix");
    aShaders[0].u_TexSamplerId      = -1;
//  aShaders[0].u_BmpSamplerId      = glGetUniformLocation(aShader.programId, "u_BmpSampler");
    if (aShaders[0].a_VertexId == -1    || aShaders[0].u_MvpMatrixId == -1       || aShaders[0].a_ColorId == -1
        /* || aShaders[0].a_NormalId == -1 || aShaders[0].u_NormalMatrixId == -1*/) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to loadShaders() id=%d\n", aShaders[0].programId);
        return false;
    }

    /* テクスチャ用 */
    aShaders[1].programId = (int)GLES2::loadShaders(texvsh, texfsh);
    if(aShaders[1].programId == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to loadShaders() id=%d\n", aShaders[1].programId);
        return false;
    }

    aShaders[1].a_VertexId          = glGetAttribLocation(aShaders[1].programId, "a_Vertex");
    aShaders[1].a_NormalId          = glGetAttribLocation(aShaders[1].programId, "a_Normal");
    aShaders[1].a_ColorId           = glGetAttribLocation(aShaders[1].programId, "a_Color");
    aShaders[1].a_TexCoordId        = glGetAttribLocation(aShaders[1].programId, "a_TexCoord");
    aShaders[1].u_MvpMatrixId       = glGetUniformLocation(aShaders[1].programId, "u_MvpMatrix");
    aShaders[1].u_NormalMatrixId    = glGetUniformLocation(aShaders[1].programId, "u_NormalMatrix");
    aShaders[1].u_TexSamplerId      = glGetUniformLocation(aShaders[1].programId, "u_TexSampler");
//  aShader.u_BmpSamplerId      = glGetUniformLocation(aShader.programId, "u_BmpSampler");
    if (aShaders[1].a_VertexId == -1 || aShaders[1].u_MvpMatrixId == -1 || aShaders[1].a_TexCoordId == -1 || aShaders[1].u_TexSamplerId == -1
        /*|| aShaders[1].a_NormalId == -1 || aShaders[1].u_NormalMatrixId == -1 || aShaders[1].a_ColorId == -1*/) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to loadShaders() id=%d\n", aShaders[1].programId);
        return false;
    }

    return true;
}

unsigned int GLES2::loadShaders(const std::vector<char> &vsh, const std::vector<char> &fsh) {
    /************************/
    /* バーテックスシェーダ */
    /************************/
    /* コンパイル */
    unsigned int vshaderid = GLES2::compileShader(vsh, GL_VERTEX_SHADER);
    if(vshaderid == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to compile vertex shader\n");
        return 0;
    }

    /************************/
    /* フラグメントシェーダ */
    /************************/
    /* コンパイル */
    unsigned int fshaderid = GLES2::compileShader(fsh, GL_FRAGMENT_SHADER);
    if(fshaderid == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to compile fragment shader\n");
        return 0;
    }

    /**************************/
    /* Create shader program. */
    /**************************/
    unsigned int programid = glCreateProgram();

    /* Attach shader to program. */
    glAttachShader(programid, vshaderid);	/* vertex shader */
    glAttachShader(programid, fshaderid);	/* fragment shader */

    /*****************/
    /* Link program. */
    /*****************/
    bool ret3 = GLES2::linkProgram(programid);
    if (!ret3) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to link program : %d", programid);
        glDetachShader(programid, vshaderid);
        glDetachShader(programid, fshaderid);
        glDeleteShader(vshaderid);
        glDeleteShader(fshaderid);
        glDeleteProgram(programid);
        return 0;
    }

    /************/
    /* 後片付け */
    /************/
    glDetachShader(programid, vshaderid);
    glDetachShader(programid, fshaderid);
    glDeleteShader(vshaderid);
    glDeleteShader(fshaderid);

    return programid;
}

#include <iostream>
#include <fstream>


unsigned int GLES2::compileShader(const std::vector<char> &aShaderSource, const int aType) {
    /* シェーダーオブジェクト生成 */
    unsigned int shaderid = glCreateShader(aType);
    if (shaderid == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "ERROR: Fail!! glCreateShader(0x%x)\n", aType);
        return 0;
    }

    /* ソースコードをGLESに送る */
    char src[2048] = {0};
    std::copy(std::begin(aShaderSource), std::end(aShaderSource), std::begin(src));
    const char *source = (const char *)src;
    glShaderSource(shaderid, 1, &source, nullptr);

    /* コンパイル */
    glCompileShader(shaderid);

    /* コンパイルの結果判定 */
    int isCompiled = 0;
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        /* 結果文字列長取得 */
        int maxLength = 0;
        glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &maxLength);
        /* 格納領域確保 */
        char *gllogstr = (char*)malloc(maxLength);
        /* 結果文字列取得 */
        glGetShaderInfoLog(shaderid, maxLength, &maxLength, gllogstr);
        if (aType == GL_VERTEX_SHADER) {
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "VertexShader : %s\n", gllogstr);
        }
        else {
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "FragmentShader : %s\n", gllogstr);
        }
        /* 格納領域解放 */
        free(gllogstr);
        /* シェーダーも解放 */
        glDeleteShader(shaderid);
        return 0;
    }

    return shaderid;
}

bool GLES2::linkProgram(const unsigned int programId) {
    glLinkProgram(programId);

#if defined(_DEBUG)
    int logLength;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		char *logstr2 = (char*)malloc(logLength);
		glGetProgramInfoLog(programId, logLength, &logLength, logstr2);
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Program link log : %s\n", logstr2);
		free(logstr2);
	}
#endif

    int status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    return status != 0;
}

bool GLES2::BufferInit(GlBufferObj &aVertex) {
    GLuint ids[4] = {0};
    glGenBuffers(4, ids);

    aVertex.VertexBufId = ids[0];   /* 頂点バッファ */
    aVertex.NormalBuffId= ids[1];   /* 法線バッファ */
    aVertex.ColorBuffId = ids[2];   /* カラーバッファ */
    aVertex.TexCoordBuffId = ids[3];/* Uvバッファ */

    return true;
}

void GLES2::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GlRenderData &SystemData = GlRenderData::GetIns();

    std::vector<DrawInfo> &DrawInfos = SystemData.mDrawInfos;
    for(auto & drawinfo : DrawInfos) {
        GlShaderObj &shaderobj = (drawinfo.mTexBinData == nullptr) ? SystemData.mGlShaderObjs[0] : SystemData.mGlShaderObjs[1];

        glUseProgram(shaderobj.programId);
        if(drawinfo.mTexBinData != nullptr)
            glUniform1i(shaderobj.u_TexSamplerId, 0);

        /* 頂点 */
        assert((drawinfo.mVerArrySize == drawinfo.mVirtexs.size()*3*sizeof(float)) && "aaaaa");
        glBindBuffer(GL_ARRAY_BUFFER, SystemData.mGlBufferObj.VertexBufId);
        glBufferData(GL_ARRAY_BUFFER, drawinfo.mVerArrySize, drawinfo.mVerArry, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(shaderobj.a_VertexId, 3, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(shaderobj.a_VertexId);

        /* 法線 */
        assert((drawinfo.mNorArrySize == drawinfo.mNormals.size()*3*sizeof(float)) && "aaaaa");
        glBindBuffer(GL_ARRAY_BUFFER, SystemData.mGlBufferObj.NormalBuffId);
        glBufferData(GL_ARRAY_BUFFER, drawinfo.mNorArrySize, drawinfo.mNorArry, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(shaderobj.a_NormalId, 3, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(shaderobj.a_NormalId);

        /* カラー */
        assert((drawinfo.mClrArrySize == drawinfo.mColors.size()*4*sizeof(float)) && "aaaaa");
        glBindBuffer(GL_ARRAY_BUFFER, SystemData.mGlBufferObj.ColorBuffId);
        glBufferData(GL_ARRAY_BUFFER, drawinfo.mClrArrySize, drawinfo.mClrArry, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(shaderobj.a_ColorId, 4, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(shaderobj.a_ColorId);

        if(drawinfo.mTexBinData != nullptr) {
            /* UV設定 */
            glBindBuffer(GL_ARRAY_BUFFER, SystemData.mGlBufferObj.TexCoordBuffId);
            glBufferData(GL_ARRAY_BUFFER, drawinfo.mUVs.size()*2*sizeof(float), drawinfo.mUvsArry, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(shaderobj.a_TexCoordId, 2, GL_FLOAT, false, 0, 0);
            glEnableVertexAttribArray(shaderobj.a_TexCoordId);
            /* テクスチャ画像設定 */
            glBindTexture(GL_TEXTURE_2D, SystemData.mTexId);
//            __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa mTexWidth=%d mTexHeight=%d imgサイズ=%d %d drawinfo.mTexBinData=0x%p", drawinfo.mTexWidth, drawinfo.mTexHeight, drawinfo.mTexBinSize, drawinfo.mTexWidth*drawinfo.mTexHeight*4, drawinfo.mTexBinData);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, drawinfo.mTexWidth, drawinfo.mTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, drawinfo.mTexBinData);
        }

        m::MatVec::LoadIdentity(SystemData.mModelMatrix);
        GLES2::calcCordinate(shaderobj, SystemData.mModelMatrix, SystemData.mVpMatrix, SystemData.mMvpMatrix, SystemData.mNormalMatrix);
        // 描画
//        __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa vertexサイズ=%d", (int)(drawinfo.mVirtexs.size()));
        glDrawArrays(GL_TRIANGLES, 0, drawinfo.mVirtexs.size());
    }
    return;
}

void GLES2::calcCordinate(GlShaderObj &aShaderObj, std::array<float, 16> &aModelMatrix, std::array<float, 16> &aViewProjMatrix, std::array<float, 16> &aMvpMatrix, std::array<float, 16> &aNormalMatrix) {
    GlRenderData &RenderData = GlRenderData::GetIns();

    aModelMatrix = m::MatVec::GetRotatef(-RenderData.mTouchAngleX, 1.0f, 0.0f, 0.0f);
    m::MatVec::Rotatef(aModelMatrix, RenderData.mTouchAngleY, 0.0f, 1.0f, 0.0f);
    m::MatVec::Translatef(aModelMatrix, 0.0f, -150.0f, 0.0f);
    m::MatVec::Scalef(aModelMatrix, RenderData.mScale, RenderData.mScale, RenderData.mScale);

    // 法線の変換行列を計算し、u_NormalMatrixに設定する
    std::array<float, 16> inv = {0};
    bool ret = m::MatVec::invertf(inv, aModelMatrix);
    if(ret) {
        m::MatVec::transposef(aNormalMatrix, inv);
        glUniformMatrix4fv(aShaderObj.u_NormalMatrixId, 1, false, aNormalMatrix.data());
    }
    else {
        aNormalMatrix = m::Matrix4f::IDENTITY;
        glUniformMatrix4fv(aShaderObj.u_NormalMatrixId, 1, false, aNormalMatrix.data());
    }

    /* モデルビュー投影行列を計算し、u_MvpMatrixに設定する */
    m::MatVec::MultMatrixf(aMvpMatrix, aViewProjMatrix, aModelMatrix);
    glUniformMatrix4fv(aShaderObj.u_MvpMatrixId, 1, false, aMvpMatrix.data());
}

