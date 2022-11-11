#pragma warning(disable : 4819)
//
// Created by jun on 2021/06/08.
//
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#ifdef __ANDROID__
#include <android/log.h>
#include <GLES2/gl2.h>
#else   /* __ANDROID__ */
#include <windows.h>
#include <GL/gl.h>
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/include/GL/wglext.h"
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/include/GL2/gl2.h"
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/include/GL/glext.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 6031)
#endif  /* __ANDROID__ */
#include "../CG3DCom.h"
#include "MQO.h"

std::tuple<bool, MqoInfo> MQO::init(const std::vector<char> &MqoModelData) {
    MqoInfo retMqoInfo;
    bool ret = Load(MqoModelData, retMqoInfo);
    if(!ret) {
        __android_log_print(ANDROID_LOG_DEBUG,"aaaaa","Load(%s) Failed!!\n", "vignette_ppp.mqo");
    }

    return {ret, retMqoInfo};
}

bool MQO::Load(const std::vector<char> &MqoModelData, MqoInfo &aMqoInfo) {
    /* 行でsplit */
    std::vector<std::string> mqoContents;
    std::stringstream ss(MqoModelData.data());
    std::string lineStr;
    while (std::getline(ss, lineStr, '\n')) {
        if (!lineStr.empty())
            mqoContents.push_back(lineStr);
    }

    /* チャンク読込 */
    for(unsigned int lpct = 0; lpct < mqoContents.size(); lpct++) {
        if(strstr(mqoContents[lpct].c_str(),"Material") != nullptr) {
            LoadMaterial(mqoContents, lpct, aMqoInfo.mMqoMaterials);
        }
        else if(strstr(mqoContents[lpct].c_str(),"Object") != nullptr) {
            LoadObject(mqoContents, lpct, aMqoInfo.mMqoObjects);
        }
    }

    return true;
}

void MQO::LoadMaterial(std::vector<std::string> &contents, unsigned int &lpct, std::vector<MqoMaterial> &aMaterial) {
    int numofMaterial;
    sscanf(contents[lpct++].c_str(), "Material %d", &numofMaterial);  /* マテリアル総数 */

    for(int matid = 0; matid < numofMaterial; lpct++,matid++) {
        MqoMaterial material;
        material.MaterialID = matid;                        /* マテリアルID */

        char matname[256] = {0};
        sscanf(contents[lpct].c_str(), "\t\"%[^\"]", matname);      /* マテリアル名 */
        material.MaterialName = matname;

        char* buf2 = const_cast<char*>(strstr(contents[lpct].c_str(),"vcol("));
        if(buf2 == nullptr)
            buf2 = const_cast<char*>(contents[lpct].c_str());
        else
            buf2 += sizeof("vcol(1)");

        if((buf2 = strstr(buf2,"col(")) != nullptr ){/* マテリアルカラー */
            sscanf(buf2,"col(%f %f %f %f)",  &material.Color.r, &material.Color.g, &material.Color.b, &material.Color.a);
        }
        if((buf2 = strstr(buf2,"dif(")) != nullptr ){/* ディフューズ */
            sscanf(buf2,"dif(%f)",  &material.ReflectionColor.diffuse);
        }
        if((buf2 = strstr(buf2,"amb(")) != nullptr ){/* アンビエント格納 */
            sscanf(buf2,"amb(%f)",  &material.ReflectionColor.ambient);
        }
        if((buf2 = strstr(buf2,"emi(")) != nullptr ){/* エミッション格納 */
            sscanf(buf2,"emi(%f)",  &material.ReflectionColor.emission);
        }
        if((buf2 = strstr(buf2,"spc(")) != nullptr ){/* スペキュラー格納 */
            sscanf(buf2,"spc(%f)",  &material.ReflectionColor.specular);
        }
        if((buf2 = strstr(buf2,"power(")) != nullptr ){/* shiness格納 */
            sscanf(buf2,"power(%f)",  &material.Power);
        }
        if((buf2 = strstr(buf2,"tex(")) != nullptr ){/* テクスチャ名(模様マッピング)格納 */
            char texname[256] = {0};
            sscanf(buf2,"tex(\"%[^\"])", texname);
            material.TexName = texname;
        }
        else {
            aMaterial.push_back(material);
            continue;
        }
        if((buf2 = strstr(buf2,"aplane(")) != nullptr ){/* 透明マッピング */
            char aplname[256] = {0};
            sscanf(buf2,"aplane(\"%[^\"])", aplname);
            material.AlpName = aplname;
        }
        else {
            aMaterial.push_back(material);
            continue;
        }
        if((buf2 = strstr(buf2,"bump(")) != nullptr ){/* 凹凸マッピング ※読込むだけ。使い方がわからんから今回は非対応 */
            char bmpname[256] = {0};
            sscanf(buf2,"bump(\"%[^\"])", bmpname);
            material.BmpName = bmpname;
        }
        else {
            aMaterial.push_back(material);
            continue;
        }

        aMaterial.push_back(material);
    }

    return;
}

void MQO::LoadObject(std::vector<std::string> &contents, unsigned int &lpct, std::vector<MqoObject> &aObjects) {
    char objname[256] = {0};
    sscanf(contents[lpct++].c_str(), "Object \"%[^\"] {", objname);      /* メッシュ名 */
    MqoObject mqoObject;
    mqoObject.Name = objname;

    for(; lpct < contents.size(); lpct++) {
        if(strstr(contents[lpct].c_str(),"visible") != nullptr) {
            sscanf(contents[lpct].c_str(), "\tvisible %d", &mqoObject.visible);     /* 可視/不可視 */
        }
        else if(strstr(contents[lpct].c_str(),"shading") != nullptr) {
            sscanf(contents[lpct].c_str(), "\tshading %d", &mqoObject.shading);
        }
        else if(strstr(contents[lpct].c_str(),"color") != nullptr) {
            mqoObject.color.a = 1;
            sscanf(contents[lpct].c_str(), "\tcolor %f %f %f", &mqoObject.color.r,&mqoObject.color.g,&mqoObject.color.b);   /* カラー */
        }
        else if(strstr(contents[lpct].c_str(),"vertex") != nullptr) {
            LoadVertex(contents, lpct, mqoObject.Vertex);
        }
        else if(strstr(contents[lpct].c_str(),"facet") != nullptr) {
            continue;   /* 何もしない。 facetが先に引っかかってしまうのでここで対処 */
        }
        else if(strstr(contents[lpct].c_str(),"face") != nullptr) {
            LoadFace(contents, lpct, mqoObject.TriangleData, mqoObject.QuadData);
            setNormal(mqoObject);

            if(mqoObject.visible == 15)   /* 可視の時に追加 */
                aObjects.push_back(mqoObject);

            break;
        }
    }

    return;
}

void MQO::LoadVertex(std::vector<std::string> &contents, unsigned int &lpct, std::vector<m::Vector3f> &aVertexs) {
    int numofVertexs;
    const char *vertexStr = strstr(contents[lpct++].c_str(),"vertex");
    sscanf(vertexStr, "vertex %d {", &numofVertexs);  /* 頂点数総数 */

    for(int vct = 0; vct < numofVertexs; vct++) {
        m::Vector3f v;
        sscanf(contents[lpct++].c_str(),"%f %f %f",&v.x,&v.y,&v.z);
        aVertexs.push_back(v);
    }
    return;
}

void MQO::LoadFace(std::vector<std::string> &contents, unsigned int &lpct, std::vector<m::Triangle> &aTriangles, std::vector<m::Quad> &aQuadRilaterals) {
    int numofFaces;
    const char *faceStr = strstr(contents[lpct++].c_str(), "face");
    sscanf(faceStr, "face %d {", &numofFaces);  /* ポリゴン総数 */

    for(int fct = 0; fct < numofFaces; fct++) {
        int facetype;
        sscanf(contents[lpct].c_str(), "%d", &facetype);
        if(facetype==3){
            // 3 V(0 1 2) M(16) UV(0.04167 0.00000 0.00000 0.12500 0.08333 0.12500) COL(4289265663 4284904555 4284904555)
            m::Triangle mqoTriangle;
            char *buf2 = const_cast<char*>(contents[lpct++].c_str());
            if((buf2 = strstr(buf2, "V(")) != nullptr ){
                sscanf(buf2,"V(%d %d %d)",  &mqoTriangle.Index[0], &mqoTriangle.Index[1], &mqoTriangle.Index[2]);
            }
            if((buf2 = strstr(buf2,"M(")) != nullptr ){
                sscanf(buf2,"M(%d)",  &mqoTriangle.MaterialID);
            }
            if((buf2 = strstr(buf2,"UV(")) != nullptr ){
                sscanf(buf2,"UV(%f %f %f %f %f %f)",
                       &mqoTriangle.uv[0].u, &mqoTriangle.uv[0].v,
                       &mqoTriangle.uv[1].u, &mqoTriangle.uv[1].v,
                       &mqoTriangle.uv[2].u, &mqoTriangle.uv[2].v);
            }
            aTriangles.push_back(mqoTriangle);
        }
        else if(facetype==4){
            // 4 V(1 13 14 2) M(16) UV(0.00000 0.12500 0.00000 0.25000 0.08333 0.25000 0.08333 0.12500) COL(4284904555 4282138671 4282138671 4284904555)
            m::Quad mqoQuadRilateral;
            char *buf2 = const_cast<char*>(contents[lpct++].c_str());
            if((buf2 = strstr(buf2,"V(")) != nullptr ){
                sscanf(buf2,"V(%d %d %d %d)",
                       &mqoQuadRilateral.Index[0], &mqoQuadRilateral.Index[1],
                       &mqoQuadRilateral.Index[2], &mqoQuadRilateral.Index[3]);
            }
            if((buf2 = strstr(buf2,"M(")) != nullptr ){
                sscanf(buf2,"M(%d)",  &mqoQuadRilateral.MaterialID);
            }
            if((buf2 = strstr(buf2,"UV(")) != nullptr ){
                sscanf(buf2,"UV(%f %f %f %f %f %f %f %f)",
                       &mqoQuadRilateral.uv[0].u, &mqoQuadRilateral.uv[0].v,
                       &mqoQuadRilateral.uv[1].u, &mqoQuadRilateral.uv[1].v,
                       &mqoQuadRilateral.uv[2].u, &mqoQuadRilateral.uv[2].v,
                       &mqoQuadRilateral.uv[3].u, &mqoQuadRilateral.uv[3].v);
            }
            aQuadRilaterals.push_back(mqoQuadRilateral);
        }
    }

    return;
}

bool MQO::remakeDrawInfo(MqoInfo &aMqoInfo, std::vector<DrawInfo> &aDrawInfos) {
    aDrawInfos.clear();

    /* マテリアル情報の設定 */
    for(unsigned int lpct = 0; lpct < aMqoInfo.mMqoMaterials.size(); lpct++) {
        aDrawInfos.emplace_back();
    }

    for(unsigned int lpct = 0; lpct < aMqoInfo.mMqoObjects.size(); lpct++) {
        MqoObject &mqobbject = aMqoInfo.mMqoObjects[lpct];

        /* 3角ポリゴンの頂点,法線,UVを再生成 */
        for(unsigned int tricnt = 0; tricnt < mqobbject.TriangleData.size(); tricnt++) {
            m::Triangle &triangle = mqobbject.TriangleData[tricnt];
            /* 頂点設定 */
            aDrawInfos[triangle.MaterialID].mVirtexs.push_back(mqobbject.Vertex[triangle.Index[0]]);
            aDrawInfos[triangle.MaterialID].mVirtexs.push_back(mqobbject.Vertex[triangle.Index[1]]);
            aDrawInfos[triangle.MaterialID].mVirtexs.push_back(mqobbject.Vertex[triangle.Index[2]]);
            /* UV設定 */
            aDrawInfos[triangle.MaterialID].mUVs.push_back(triangle.uv[0]);
            aDrawInfos[triangle.MaterialID].mUVs.push_back(triangle.uv[1]);
            aDrawInfos[triangle.MaterialID].mUVs.push_back(triangle.uv[2]);
            /* 法線 */
            if(mqobbject.shading == 1) {    /* グローシェーディング */
                aDrawInfos[triangle.MaterialID].mNormals.push_back(mqobbject.Normal[triangle.Index[0]]);
                aDrawInfos[triangle.MaterialID].mNormals.push_back(mqobbject.Normal[triangle.Index[1]]);
                aDrawInfos[triangle.MaterialID].mNormals.push_back(mqobbject.Normal[triangle.Index[2]]);
            }
            else {                          /* フラットシェーディング */
                aDrawInfos[triangle.MaterialID].mNormals.push_back(triangle.Normal);
                aDrawInfos[triangle.MaterialID].mNormals.push_back(triangle.Normal);
                aDrawInfos[triangle.MaterialID].mNormals.push_back(triangle.Normal);
            }
            /* カラー設定 */
            int matid = triangle.MaterialID;
            auto findit = std::find_if(aMqoInfo.mMqoMaterials.begin(), aMqoInfo.mMqoMaterials.end(), [matid](const MqoMaterial &item){return item.MaterialID==matid;});
            assert((findit != aMqoInfo.mMqoMaterials.end()) &&
                	CG3D::format("該当Materialが見つからない。ありえない。matid=", matid).c_str());
            m::Color4 &color = (*findit).Color;
            if(color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
                aDrawInfos[triangle.MaterialID].mColors.push_back(mqobbject.color);
                aDrawInfos[triangle.MaterialID].mColors.push_back(mqobbject.color);
                aDrawInfos[triangle.MaterialID].mColors.push_back(mqobbject.color);
            }
            else {
                aDrawInfos[triangle.MaterialID].mColors.push_back(color);
                aDrawInfos[triangle.MaterialID].mColors.push_back(color);
                aDrawInfos[triangle.MaterialID].mColors.push_back(color);
            }
        }

        /* 4角ポリゴンの頂点,法線,UVを再生成 */
        for(unsigned int quadcnt = 0; quadcnt < mqobbject.QuadData.size(); quadcnt++) {
            m::Quad &quadrilateral = mqobbject.QuadData[quadcnt];
            /* 頂点設定 */
            aDrawInfos[quadrilateral.MaterialID].mVirtexs.push_back(mqobbject.Vertex[quadrilateral.Index[0]]);
            aDrawInfos[quadrilateral.MaterialID].mVirtexs.push_back(mqobbject.Vertex[quadrilateral.Index[1]]);
            aDrawInfos[quadrilateral.MaterialID].mVirtexs.push_back(mqobbject.Vertex[quadrilateral.Index[2]]);
            aDrawInfos[quadrilateral.MaterialID].mVirtexs.push_back(mqobbject.Vertex[quadrilateral.Index[0]]);
            aDrawInfos[quadrilateral.MaterialID].mVirtexs.push_back(mqobbject.Vertex[quadrilateral.Index[2]]);
            aDrawInfos[quadrilateral.MaterialID].mVirtexs.push_back(mqobbject.Vertex[quadrilateral.Index[3]]);
            /* UV設定 */
            aDrawInfos[quadrilateral.MaterialID].mUVs.push_back(quadrilateral.uv[0]);
            aDrawInfos[quadrilateral.MaterialID].mUVs.push_back(quadrilateral.uv[1]);
            aDrawInfos[quadrilateral.MaterialID].mUVs.push_back(quadrilateral.uv[2]);
            aDrawInfos[quadrilateral.MaterialID].mUVs.push_back(quadrilateral.uv[0]);
            aDrawInfos[quadrilateral.MaterialID].mUVs.push_back(quadrilateral.uv[2]);
            aDrawInfos[quadrilateral.MaterialID].mUVs.push_back(quadrilateral.uv[3]);
            /* 法線 */
            if(mqobbject.shading == 1) {    /* グローシェーディング */
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(mqobbject.Normal[quadrilateral.Index[0]]);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(mqobbject.Normal[quadrilateral.Index[1]]);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(mqobbject.Normal[quadrilateral.Index[2]]);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(mqobbject.Normal[quadrilateral.Index[0]]);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(mqobbject.Normal[quadrilateral.Index[2]]);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(mqobbject.Normal[quadrilateral.Index[3]]);
            }
            else {                          /* フラットシェーディング */
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(quadrilateral.Normal);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(quadrilateral.Normal);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(quadrilateral.Normal);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(quadrilateral.Normal);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(quadrilateral.Normal);
                aDrawInfos[quadrilateral.MaterialID].mNormals.push_back(quadrilateral.Normal);
            }
            /* カラー設定 */
            int matid = quadrilateral.MaterialID;
            auto findit = std::find_if(aMqoInfo.mMqoMaterials.begin(), aMqoInfo.mMqoMaterials.end(), [matid](const MqoMaterial &item){return item.MaterialID==matid;});
            assert((findit != aMqoInfo.mMqoMaterials.end()) && 
                CG3D::format("該当Materialが見つからない。ありえない。matid=", matid, "(", __LINE__, ")").c_str());

            m::Color4 &color = (*findit).Color;
            if(color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(mqobbject.color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(mqobbject.color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(mqobbject.color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(mqobbject.color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(mqobbject.color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(mqobbject.color);
            }
            else {
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(color);
                aDrawInfos[quadrilateral.MaterialID].mColors.push_back(color);
            }
        }
    }

    for(unsigned int lpct = 0; lpct < aMqoInfo.mMqoMaterials.size(); lpct++) {
        if(aDrawInfos[lpct].mVirtexs.size() == aDrawInfos[lpct].mUVs.size() &&
           aDrawInfos[lpct].mNormals.size() == aDrawInfos[lpct].mColors.size() &&
           aDrawInfos[lpct].mVirtexs.size() == aDrawInfos[lpct].mNormals.size() ) {
            /* OK */
        }
        else {
            /* NG*/
            assert(false && "COUNT 不整合!!");
        }
    }

    for(unsigned int lpct = 0; lpct < aDrawInfos.size(); lpct++) {
        DrawInfo &drawinfo = aDrawInfos[lpct];
        drawinfo.mVerArrySize = (int)(drawinfo.mVirtexs.size() * 3 * sizeof(float));
        drawinfo.mVerArry = vector2array(drawinfo.mVirtexs);
        if(drawinfo.mVerArry == nullptr) {
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa","drawinfo.mVerArry == NULL!! lpct=%d\n",lpct);
            aDrawInfos.clear();
            return false;
        }
        drawinfo.mNorArrySize = (int)(drawinfo.mNormals.size() * 3 * sizeof(float));
        drawinfo.mNorArry = vector2array(drawinfo.mNormals);
        if(drawinfo.mNorArry == nullptr) {
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa","drawinfo.mNorArry == NULL!! lpct=%d\n",lpct);
            aDrawInfos.clear();
            return false;
        }
        drawinfo.mUvsArrySize = (int)(drawinfo.mUVs.size() * 2 * sizeof(float));
        drawinfo.mUvsArry = vector2array(drawinfo.mUVs);
        if(drawinfo.mUvsArry == nullptr) {
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa","drawinfo.mUvsArry == NULL!! lpct=%d\n",lpct);
            aDrawInfos.clear();
            return false;
        }
        drawinfo.mClrArrySize = (int)(drawinfo.mColors.size() * 4 * sizeof(float));
        drawinfo.mClrArry = vector2array(drawinfo.mColors);
        if(drawinfo.mClrArry == nullptr) {
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa","drawinfo.mClrArry == NULL!! lpct=%d\n",lpct);
            aDrawInfos.clear();
            return false;
        }
    }

    return true;
}

float *MQO::vector2array(std::vector<m::Vector3f> &vector) {
    float *ret = (float*)malloc(vector.size()*3*sizeof(float));
    if(ret == nullptr) return nullptr;
    for(unsigned int lpct = 0,fltcnt = 0; lpct < vector.size(); lpct++,fltcnt+=3) {
        ret[fltcnt + 0] = vector[lpct].x;
        ret[fltcnt + 1] = vector[lpct].y;
        ret[fltcnt + 2] = vector[lpct].z;
    }
    return ret;
}

float *MQO::vector2array(std::vector<m::UV> &vector) {
    float *ret = (float*)malloc(vector.size()*2*sizeof(float));
    for(unsigned int lpct = 0,fltcnt = 0; lpct < vector.size(); lpct++,fltcnt+=2) {
        ret[fltcnt + 0] = vector[lpct].u;
        ret[fltcnt + 1] = vector[lpct].v;
    }
    return ret;
}

float *MQO::vector2array(std::vector<m::Color4> &vector) {
    float *ret = new float[vector.size()*4*sizeof(float)];
    for(unsigned int lpct = 0,fltcnt = 0; lpct < vector.size(); lpct++,fltcnt+=4) {
        ret[fltcnt + 0] = vector[lpct].r;
        ret[fltcnt + 1] = vector[lpct].g;
        ret[fltcnt + 2] = vector[lpct].b;
        ret[fltcnt + 3] = vector[lpct].a;
    }
    return ret;
}

void MQO::setNormal(MqoObject &mqoobject) {
    mqoobject.Normal.clear();
    mqoobject.Normal.resize(mqoobject.Vertex.size());

    /* 3角形ポリゴン */
    for(unsigned int lpct = 0; lpct < mqoobject.TriangleData.size(); lpct++){
        m::Triangle mqotriangle = mqoobject.TriangleData[lpct];
        m::Vector3f v0 = mqoobject.Vertex[mqotriangle.Index[0]];
        m::Vector3f v1 = mqoobject.Vertex[mqotriangle.Index[1]];
        m::Vector3f v2 = mqoobject.Vertex[mqotriangle.Index[2]];
        m::Vector3f normal = calcNormal(v0, v1, v2);
        /* 3角形ポリゴンの法線ベクトルを設定 */
        mqotriangle.Normal = normal;
        /* 3角形ポリゴンを構成する頂点に法線ベクトルを設定 */
        mqoobject.Normal[mqotriangle.Index[0]] += normal;   /* 加算していることに注意 */
        mqoobject.Normal[mqotriangle.Index[1]] += normal;
        mqoobject.Normal[mqotriangle.Index[2]] += normal;
    }

    /* 4角形ポリゴン */
    for(unsigned int lpct = 0; lpct < mqoobject.QuadData.size(); lpct++){
        m::Quad quadrilateral = mqoobject.QuadData[lpct];
        m::Vector3f v0 = mqoobject.Vertex[quadrilateral.Index[0]];
        m::Vector3f v1 = mqoobject.Vertex[quadrilateral.Index[1]];
        m::Vector3f v2 = mqoobject.Vertex[quadrilateral.Index[2]];
        m::Vector3f normal = calcNormal(v0, v1, v2);
        /* 3角形ポリゴンの法線ベクトルを設定 */
        quadrilateral.Normal = normal;
        /* 3角形ポリゴンを構成する頂点に法線ベクトルを設定 */
        mqoobject.Normal[quadrilateral.Index[0]] += normal;   /* 加算していることに注意 */
        mqoobject.Normal[quadrilateral.Index[1]] += normal;
        mqoobject.Normal[quadrilateral.Index[2]] += normal;
        mqoobject.Normal[quadrilateral.Index[3]] += normal;
    }

    return;
}

typedef struct BMPHEADER {
    char    bm[2];      /* "BM" */
    int     size;       /* 全体サイズ */
    short   reserve1;   /* 予約領域1 */
    short   reserve2;   /* 予約領域2 */
    int     headersize; /* ファイルヘッダサイズ */
    int     infosize;   /* 情報ヘッダのサイズ */
    int     width;      /* 画像幅 */
    int     hight;      /* 画像高さ */
    short   numofplane; /* プレーン数(常に1) */
    short   bitdepth;   /* ビットサイズ(今回は24しか対応しない) */
    int     comptype;   /* 圧縮タイプ(今回は非圧縮しか対応しない) */
    int     binsize;    /* 画像サイズ(width*hight*RGB(3)) */
    int     hResolution;/* 水平解像度(未使用) */
    int     vResolution;/* 垂直解像度(未使用) */
    int     paletcolors;/* パレットの色数(未使用) */
    int     paletindex; /* パレットのインデックス(未使用) */
} BmpHeader;

bool MQO::TextureInit(const std::map<std::string, std::vector<char>> &AssetDatas, const std::vector<MqoMaterial> &aMqoMaterial, std::vector<DrawInfo> &aDrawInfos) {
    //glGenTextures(1, &texid);

    /* 画像イメージ読込(BMPのみ対応) */
    for(unsigned int lpct = 0; lpct < aMqoMaterial.size(); lpct++) {
        if(aMqoMaterial[lpct].TexName == "")
            continue;

        /****************/
        /* 模様テクスチャ */
        /****************/
        /* 画像の生データ全取得 */
        const char *lTexData = AssetDatas.at(aMqoMaterial[lpct].TexName).data();
        if(lTexData == nullptr)
            continue;

        /* BMP画像のヘッダ解析 */
        BmpHeader colbmpheader;
        memcpy(&colbmpheader, lTexData, sizeof("BM"));
        if(memcmp(colbmpheader.bm,"BM",0x02) != 0) {   /* bmp形式でなければ、読飛ばし */
            __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "Not Supported=%c%c\n", colbmpheader.bm[0],colbmpheader.bm[1]);
            continue;
        }

        memcpy(&colbmpheader.size, &lTexData[2], sizeof(BmpHeader)-sizeof("BM")-sizeof(short));
        if(colbmpheader.bitdepth != 24 || colbmpheader.comptype != 0) {
            /* RGB24ビットしか対応しない、無圧縮形式しか対応しない */
            __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "Not Supported colordepth=%d comptype=%d\n", colbmpheader.bitdepth, colbmpheader.comptype);
            continue;
        }

        aDrawInfos[lpct].mTexWidth = colbmpheader.width;
        aDrawInfos[lpct].mTexHeight = colbmpheader.hight;

        /* BGRデータ取得 */
        char* buf = (char*)malloc(colbmpheader.binsize);
        memcpy(buf, &(lTexData[sizeof(BmpHeader)-sizeof(short)]), colbmpheader.binsize);

        /**********************/
        /* アルファテクスチャ */
        /**********************/
        char* alpbuf = nullptr;
        if(!aMqoMaterial[lpct].AlpName.empty()) {
            const char *lAlpData = AssetDatas.at(aMqoMaterial[lpct].AlpName).data();
            if(lAlpData == nullptr)
                continue;
            /* BMP画像のヘッダ解析 */
            BmpHeader texbmpheader;
            memcpy(&texbmpheader, lAlpData, sizeof("BM"));
            if(memcmp(texbmpheader.bm,"BM",0x02) != 0) {   /* bmp形式でなければ、読飛ばし */
                __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "Not Supported=%c%c\n", texbmpheader.bm[0],texbmpheader.bm[1]);
                lAlpData = nullptr;
            }

            if(lAlpData != nullptr) {
                memcpy(&texbmpheader.size, &lAlpData[2], sizeof(BmpHeader)-sizeof("BM")-sizeof(short));
                if(texbmpheader.bitdepth != 24 || texbmpheader.comptype != 0) {
                    /* RGB24ビットしか対応しない、無圧縮形式しか対応しない */
                    __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "Not Supported colordepth=%d comptype=%d\n", texbmpheader.bitdepth, texbmpheader.comptype);
                    if(texbmpheader.binsize != colbmpheader.binsize) {
                        __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaa   binsize not same!!! colbinsize=%d texbinsize=%d\n", colbmpheader.binsize, texbmpheader.binsize);
                        assert(false && "aaaaa binsize not same!!!");
                    }
                }

                /* RGBデータ取得 */
                alpbuf = (char*)malloc(texbmpheader.binsize);
                memcpy(alpbuf, &(lAlpData[sizeof(BmpHeader)-sizeof(short)]), texbmpheader.binsize);
            }
        }

        /******************/
        /* RGBAデータを保持 */
        /******************/
        int w = colbmpheader.width, h =colbmpheader.hight;
        char *rgbabuf = (char*)malloc(w*h*4);
        for(int rgba = 0, bgr = w*h*3-3;  rgba < w*h*4; rgba+=4,bgr-=3) {
            rgbabuf[rgba + 0] = buf[bgr + 2];
            rgbabuf[rgba + 1] = buf[bgr + 1];
            rgbabuf[rgba + 2] = buf[bgr + 0];
            rgbabuf[rgba + 3] = (alpbuf!=nullptr) ? alpbuf[bgr + 0] : 0xff;
        }
        aDrawInfos[lpct].mTexBinData = rgbabuf;
        aDrawInfos[lpct].mTexBinSize = w*h*4;
    }

    //// テクスチャユニット0を有効にする
    //glActiveTexture(GL_TEXTURE0);
    //// テクスチャオブジェクトをバインドする
    //glBindTexture(GL_TEXTURE_2D, texid);
    //// テクスチャパラメータを設定する
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
}


m::Vector3f MQO::calcNormal(m::Vector3f &aV0, m::Vector3f &aV1, m::Vector3f &aV2) {
    /* aV1からaV0へのベクトル、aV1からaV2へのベクトルを求める */
    m::Vector3f v0 = aV0 - aV1;
    m::Vector3f v1 = aV2 - aV1;

    /* v0,v1の外積を求める */
    m::Vector3f ret = v0 * v1;
    ret.normalize();
    return ret;
}

/* =演算子 */
MqoObject &MqoObject::operator=(const MqoObject &rhs) {
    this->Name      = rhs.Name;
    this->visible	= rhs.visible;
    this->shading   = rhs.shading;
    this->color     = rhs.color;
    this->Vertex.reserve(rhs.Vertex.size());
    std::copy(rhs.Vertex.begin(), rhs.Vertex.end(), std::back_inserter(this->Vertex));
    this->Normal.reserve(rhs.Normal.size());
    std::copy(rhs.Normal.begin(), rhs.Normal.end(), std::back_inserter(this->Normal));
    this->TriangleData.reserve(rhs.TriangleData.size());
    std::copy(rhs.TriangleData.begin(), rhs.TriangleData.end(), std::back_inserter(this->TriangleData));
    this->QuadData.reserve(rhs.QuadData.size());
    std::copy(rhs.QuadData.begin(), rhs.QuadData.end(), std::back_inserter(this->QuadData));

    return *this;
}

DrawInfo &DrawInfo::operator=(const DrawInfo &rhs) {
    assert(false && "no imple...");

    this->mVirtexs.reserve(rhs.mVirtexs.size());
    std::copy(rhs.mVirtexs.begin(), rhs.mVirtexs.end(), std::back_inserter(this->mVirtexs));
    this->mNormals.reserve(rhs.mNormals.size());
    std::copy(rhs.mNormals.begin(), rhs.mNormals.end(), std::back_inserter(this->mNormals));
    this->mUVs.reserve(rhs.mUVs.size());
    std::copy(rhs.mUVs.begin(), rhs.mUVs.end(), std::back_inserter(this->mUVs));
    this->mColors.reserve(rhs.mColors.size());
    std::copy(rhs.mColors.begin(), rhs.mColors.end(), std::back_inserter(this->mColors));

    this->mTexWidth = rhs.mTexWidth;
    this->mTexHeight= rhs.mTexHeight;

    this->mTexBinSize = rhs.mTexBinSize;
    if(this->mTexBinSize == 0)
        this->mTexBinData = nullptr;
    else {
        this->mTexBinData = (char*)malloc(this->mTexBinSize);
        memcpy(this->mTexBinData, rhs.mTexBinData, this->mTexBinSize);
    }

    this->mVerArrySize = rhs.mVerArrySize;
    if(this->mVerArrySize == 0) {
        this->mVerArry = nullptr;
    }
    else {
        this->mVerArry = (float*)malloc(this->mVerArrySize);
        memcpy(this->mVerArry, rhs.mVerArry, this->mVerArrySize);
    }

    this->mNorArrySize = rhs.mNorArrySize;
    if(this->mNorArrySize == 0) {
        this->mNorArry = nullptr;
    }
    else {
        this->mNorArry = (float*)malloc(this->mNorArrySize);
        memcpy(this->mNorArry, rhs.mNorArry, this->mNorArrySize);
    }

    this->mUvsArrySize = rhs.mUvsArrySize;
    if(this->mUvsArrySize == 0) {
        this->mUvsArry = nullptr;
    }
    else {
        this->mUvsArry = (float*)malloc(this->mUvsArrySize);
        memcpy(this->mUvsArry, rhs.mUvsArry, this->mUvsArrySize);
    }

    this->mClrArrySize = rhs.mClrArrySize;
    if(this->mClrArrySize == 0) {
        this->mClrArry = nullptr;
    }
    else {
        this->mClrArry = (float*)malloc(this->mClrArrySize);
        memcpy(this->mClrArry, rhs.mClrArry, this->mClrArrySize);
    }

    return *this;
}

DrawInfo::~DrawInfo() {
    mVirtexs.clear();
    mNormals.clear();
    mUVs.clear();
    mColors.clear();
    if(mTexBinData!=nullptr) free(mTexBinData);
    if(mVerArry!=nullptr) free(mVerArry);
    if(mNorArry!=nullptr) free(mNorArry);
    if(mUvsArry!=nullptr) free(mUvsArry);
    if(mClrArry!=nullptr) free(mClrArry);
}
