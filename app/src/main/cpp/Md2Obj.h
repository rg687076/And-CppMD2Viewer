#ifndef CPPMD2VIEWER_MD2OBJ_H
#define CPPMD2VIEWER_MD2OBJ_H

#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <GLES2/gl2.h>
#include "Md2Parts.h"
#include "TexObj.h"

#define MD2_IDENT   (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')    /* magic number "IDP2" or 844121161 */
#define	MD2_VERSION 8                                           /* model version */

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
    std::string         mName = {0};
    std::vector<char>   mWkMd2BinData = {0};
    std::vector<char>   mWkTexBinData = {0};
    std::string         mWkVshStrData = {0};
    std::string         mWkFshStrData = {0};
    MdlData             mMdlData = {0};
    /* アニメ関連 */
    std::unordered_map<int, std::pair<int, int>> mFrameIndices = {};
    /* テクスチャ関連 */
    GLuint mTexId = -1;
    /* シェーダー関連 */
    GLuint mVbo           = -1;
    GLuint mProgramId     = -1;
    GLuint mCurPosAttrib  = -1;
    GLuint mNextPosAttrib = -1;
    GLuint mTexCoordAttrib= -1;

public:
    ~Md2ModelInfo();
    bool LoadModel();
    bool LoadTexture();
    bool LoadShaders();
};

/* Md2モデルs */
extern std::map<std::string, Md2ModelInfo> gMd2models;

class Md2Obj {
public:
    static bool Init(std::map<std::string, Md2ModelInfo> &md2models);
};

#endif //CPPMD2VIEWER_MD2OBJ_H
