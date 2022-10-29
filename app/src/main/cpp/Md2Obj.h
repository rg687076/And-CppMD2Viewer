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
typedef struct {
    int ident;      /* Magic Number. "IPD2"固定 */
    int version;    /* md2 version. 現在は8. */

    int skinwidth;  /* textureサイズ(width) */
    int skinheight; /* textureサイズ(height) */

    int framesize;  /* 1フレームのサイズ[bytes] */

    int num_skins;  /* texture数 */
    int num_xyz;    /* 頂点数(3角形とかn角形とか) */
    int num_st;     /* texture座標数 */
    int num_tris;   /* ポリゴン数 */
    int num_glcmds; /* openglコマンド数 */
    int num_frames; /* 総フレーム数 */

    int ofs_skins;  /* テクスチャ名までのoffset(64 bytes) */
    int ofs_st;     /* texture座標までのoffset(s-t) */
    int ofs_tris;   /* 頂点情報までのoffset */
    int ofs_frames; /* フレームデータまでのoffset */
    int ofs_glcmds; /* openglコマンドまでのoffset */
    int ofs_end;    /* ファイルの終端 */
} md2_t;

/* vertex情報 */
typedef struct {
    unsigned char v[3];				/* 圧縮された頂点 (x、y、z) 座標 */
    unsigned char lightnormalindex;	/* 照明の法線ベクトルへのインデックス */
} vertex_t;

/* frame情報 */
typedef struct {
    float		scale[3];     /* scale値 */
    float		translate[3]; /* 移動量 */
    char		name[16];     /* frame名称 */
    vertex_t	verts[1];     /* このフレームの最初の頂点 */
} frame_t;

/* animation情報 */
typedef struct {
    int		first_frame; /* このアニメーションの最初のフレーム */
    int		last_frame;  /* frames数 */
    int		fps;         /* fps */
} anim_t;

/* status animation */
typedef struct {
    int		startframe; /* 初回frame */
    int		endframe;   /* 最終frame */
    int		fps;        /* このanimationでのfps */

    float	curr_time;  /* current time */
    float	old_time;   /* old time */
    float	interpol;   /* 補間のパーセント(percent of interpolation) */

    int		type;       /* animationタイプ */

    int		curr_frame; /* current frame */
    int		next_frame; /* next frame */
} animState_t;

/* animation list */
typedef enum {
    STAND,
    RUN,
    ATTACK,
    PAIN_A,
    PAIN_B,
    PAIN_C,
    JUMP,
    FLIP,
    SALUTE,
    FALLBACK,
    WAVE,
    POINTE,
    CROUCH_STAND,
    CROUCH_WALK,
    CROUCH_ATTACK,
    CROUCH_PAIN,
    CROUCH_DEATH,
    DEATH_FALLBACK,
    DEATH_FALLFORWARD,
    DEATH_FALLBACKSLOW,
    BOOM,

    MAX_ANIMATIONS
} animType_t;

class glRot{
public:
    GLfloat angle;
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

typedef float vec3_t[3];

class Md2ModelInfo {
public:
    std::string         name;
    std::string         verfilename;
    std::string         texfilename;
    std::vector<char>   md2bindata;
    std::vector<char>   texbindata;
    vec3_t              *m_vertices     = nullptr;
    int                 *m_glcmds       = nullptr;
    int                 *m_lightnormals = nullptr;
    char                *m_wkbuff       = nullptr;
    TexInfo             texinfo;
    ~Md2ModelInfo();
    bool loadModel(float scale, float fps );
    bool loadSkin();
};

/* Md2モデルs */
extern std::map<std::string, Md2ModelInfo> gMd2models;

/* プロトタイプ宣言 */
extern bool Md2Init(std::map<std::string, Md2ModelInfo> &md2models);

#endif //CPPMD2VIEWER_MD2OBJ_H
