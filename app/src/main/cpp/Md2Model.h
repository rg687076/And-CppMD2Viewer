#ifndef CPPMD2VIEWER_MD2MODEL_H
#define CPPMD2VIEWER_MD2MODEL_H

#include <string>
#include <vector>

const int MD2_IDENT = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I'); /* magic number "IDP2" or 844121161 */
const int MD2_VERSION = 8;                                        /* model version */

class TmpBinData1 {
public:
    std::string         mName = {};
    std::vector<char>   mWkMd2BinData = {};
    std::vector<char>   mWkTexBinData = {};
};

class TmpBinData2 {
public:
    std::string         mName = {};
    int                 mWkWidth = 0;
    int                 mWkHeight= 0;
    std::vector<char>   mWkRgbaData = {};
};

class TmpBinData3 {
public:
    std::string         mName = {};
    std::string         mWkVshStrData = {};
    std::string         mWkFshStrData = {};
};

/* md2 header */
struct md2header {
    int magicnumber;/* Magic Number. "IPD2"固定 */
    int version;    /* md2 version. 現在は8. */

    int skinwidth;  /* textureサイズ(width) */
    int skinheight; /* textureサイズ(height) */

    int framesize;  /* 1フレームのサイズ[bytes] */

    int num_skins;      /* texture数 */
    int num_vertexs;    /* 頂点数(3角形とかn角形とか) */
    int num_st;         /* UV座標数 */
    int num_polys;      /* ポリゴン数 */
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
//    int numPolys;
//    int twidth;
//    int theight;
//    int currentFrame;
//    int nextFrame;
//    float interpol;
    std::vector<vertex>     vertexList;
    std::vector<texstcoord> st;
    std::vector<mesh>       polyIndex;
//    float x, y, z;
//    float nextX, nextY, nextZ;
//    float radius;
//    float dist_to_player;
//    int state;
//    float speed;
};

class Md2Model {
public:
    ~Md2Model();
    bool loadModel(const std::string &key, const std::vector<char> &md2bindata);   /* binデータからMd2データを読込む */
    bool loadTexture(const std::string &key, const std::vector<char> &texbindata); /* AssetsからTextureデータを読込む */

public:
    /* モデルデータ */
    MdlData                 mMdlData = {0};
    /* アニメ関連 */
    int numTotalFrames = 0;
};

#endif //CPPMD2VIEWER_MD2MODEL_H
