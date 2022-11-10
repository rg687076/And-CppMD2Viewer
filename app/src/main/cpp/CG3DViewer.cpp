//
// Created by jun on 2021/06/07.
//
#include <typeinfo>
#include <array>
#include <GLES2/gl2.h>
#include "OpenGL/GLES2.h"
#ifdef WIN32
#include "Extended.h"
#elif __ANDROID__
#include <android/log.h>
#include "AppData.h"
#include "CG3DViewer.h"
#endif

bool CG3DViewer::init() {
    const std::map<std::string, std::vector<char>> &AssetDatas = AppData::GetIns().mAssets;
    GlRenderData &RenderData = GlRenderData::GetIns();

    bool ret = GLES2::OpenGLInit();
    if(!ret) return false;
    bool ret4 = GLES2::ShaderInit(AssetDatas.at("Shaders/mqocol.vsh"), AssetDatas.at("Shaders/mqocol.fsh"), AssetDatas.at("Shaders/mqotex.vsh"), AssetDatas.at("Shaders/mqotex.fsh"), RenderData.mGlShaderObjs, RenderData.mTexId);
    if(!ret4) return false;
    bool ret5 = GLES2::BufferInit(RenderData.mGlBufferObj);
    if(!ret5) return false;

    return true;
}

void CG3DViewer::setDrawArea(int width, int height) {
    glViewport(0, 0, width, height);     // 表示領域を設定する

    // ビュー投影行列を計算
    std::array<float, 16> projMatrix = m::MatVec::GetPerspectivef(30.0, (float) width / height, 1.0, 5000.0);
    std::array<float, 16> viewMatrix = m::MatVec::GetLookAtf(0.0f, 250.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	m::MatVec::MultMatrixf(GlRenderData::GetIns().mVpMatrix, projMatrix, viewMatrix);
    return;
}

void CG3DViewer::draw() {
    GLES2::draw();
    return;
}

void CG3DViewer::setTouchAngle(float x, float y) {
    GlRenderData &RenderData = GlRenderData::GetIns();
    RenderData.mTouchAngleX = x;
    RenderData.mTouchAngleY = y;
    return;
}

void CG3DViewer::setScale(float scale) {
    GlRenderData &RenderData = GlRenderData::GetIns();
    RenderData.mScale = scale;
    return;
}
