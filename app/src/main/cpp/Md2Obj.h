#ifndef CPPMD2VIEWER_MD2OBJ_H
#define CPPMD2VIEWER_MD2OBJ_H

#include <map>
#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include "TexObj.h"

#define MD2_IDENT   (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')    /* magic number "IDP2" or 844121161 */
#define	MD2_VERSION 8                                           /* model version */

/* md2 header */
struct md2header {
    int magicnumber;/* Magic Number. "IPD2"固定 */
    int version;    /* md2 version. 現在は8. */

    int skinwidth;  /* textureサイズ(width) */
    int skinheight; /* textureサイズ(height) */

    int framesize;  /* 1フレームのサイズ[bytes] */

    int num_skins;      /* texture数 */
    int num_vertexs;    /* 頂点数(3角形とかn角形とか) */
    int num_st;         /* texture座標数 */
    int num_polys;       /* ポリゴン数 */
    int num_glcmds;     /* openglコマンド数 */
    int num_totalframes;/* 総フレーム数 */

    int offset_skins;   /* テクスチャ名までのoffset(64 bytes) */
    int offset_st;      /* uv座標までのoffset(s-t) */
    int offset_meshs;   /* mesh情報までのoffset */
    int offset_frames;  /* フレームデータまでのoffset */
    int offset_glcmds;  /* openglコマンドまでのoffset */
    int offset_end;     /* ファイルの終端 */
};

struct vertex {
    float v[3];
};

struct texstcoord {
    float s;
    float t;
};

struct texindex {
    short s;
    short t;
};

struct mesh {
    unsigned short meshIndex[3];
    unsigned short stIndex[3];
};

struct framePoint_t {
    unsigned char v[3];
    unsigned char normalIndex;
};

struct frame {
    float scale[3];
    float translate[3];
    char name[16];  /* ← 何者か不明,けっこう文字化けする,どっちにしても未使用 */
    framePoint_t fp[1];
};

struct MdlData {
    int numTotalFrames;
    int numVertexsPerFrame;
    int numPolys;
    int twidth;
    int theight;
    int currentFrame;
    int nextFrame;
    float interpol;
    std::vector<vertex>     vertexList;
    std::vector<texstcoord> st;
    std::vector<mesh>       polyIndex;
    float x, y, z;
    float nextX, nextY, nextZ;
    float radius;
    float dist_to_player;
    int state;
    float speed;
};

class Md2ModelInfo {
public:
    std::string         name = {0};
    std::string         verfilename = {0};
    std::string         texfilename = {0};
    std::vector<char>   md2bindata = {0};
    std::vector<char>   texbindata = {0};
    MdlData             mdldata = {0};
    TexData             texdata = {0};

public:
    ~Md2ModelInfo();
    bool loadModel();
    bool LoadTexture();
};

/* Md2モデルs */
extern std::map<std::string, Md2ModelInfo> gMd2models;

class Md2Obj {
public:
    static bool Init(std::map<std::string, Md2ModelInfo> &md2models);
};

#endif //CPPMD2VIEWER_MD2OBJ_H
