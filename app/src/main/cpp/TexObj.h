#ifndef CPPMD2VIEWER_TEXOBJ_H
#define CPPMD2VIEWER_TEXOBJ_H

#include <vector>

enum class FileFormat : int {
    BMP = 0,
    TGA,
};

#pragma pack(2)
typedef struct BITMAPFILEHEADER {
    char    bm[2];      /* "BM" */
    int     size;       /* 全体サイズ */
    short   reserve1;   /* 予約領域1 */
    short   reserve2;   /* 予約領域2 */
    int     offset2bin; /* ファイル先頭から画像データまでのオフセット(byte) */
} BmpFileHeader;
#pragma pack()

typedef struct BITMAPINFOHEADER {   /* Windows Bitmap */
    int     headersize; /* ヘッダサイズ */
    int     width;      /* 画像幅 */
    int     height;     /* 画像高さ */
    short   numofplane; /* プレーン数(常に1) */
    short   bitdepth;   /* ビットサイズ(1,4,8,24,32) */
    int     comptype;   /* 圧縮タイプ(今回は非圧縮しか対応しない) */
    int     binsize;    /* 画像サイズ(width*height*RGB(3)) */
    int     hResolution;/* 水平解像度(未使用) */
    int     vResolution;/* 垂直解像度(未使用) */
    int     paletcolors;/* パレットの色数(未使用) */
    int     paletindex; /* パレットのインデックス(未使用) */
} BitmapInfoHeader;

typedef struct BITMAPCOREHEADER {   /* Windows Bitmap */
    int     headersize; /* ヘッダサイズ */
    short   width;      /* 画像幅 */
    short   height;     /* 画像高さ */
    short   numofplane; /* プレーン数(常に1) */
    short   bitdepth;   /* ビットサイズ(1,4,8,24,32) */
} BitmapCoreHeader;

#pragma pack(1)
typedef struct TGAHEADER {
    unsigned char	id_length;      /* size of the structure */
    unsigned char	color_map_type; /* カラーマップタイプ */
    unsigned char	image_type;     /* データ形式 */

    short int		cm_first_entry; /* colormap開始位置 (通常0) */
    short int		cm_length;      /* colormap数 (通常0) */
    unsigned char	cm_size;        /* colormapサイズ(bits) */

    short int		is_xorigin;     /* lower left X coordinate (0固定) */
    short int		is_yorigin;     /* lower left Y coordinate (0固定) */

    short int		is_width;       /* 画像幅(pixels) */
    short int		is_height;      /* 画像高さ(pixels) */

    unsigned char	is_pixel_depth; /* 1画素のbit数: 16, 24, 32 */
    unsigned char	is_image_descriptor;// 24 bits = 0x00; 32 bits = 0x80 */

} TgaHeader;
#pragma pack()

// --------------------------------------------
// RGBTRIPLE - 24 bits pixel
// --------------------------------------------
typedef struct RGBTRIPLE {
    unsigned char	rgbtBlue;		// blue
    unsigned char	rgbtGreen;		// green
    unsigned char	rgbtRed;		// red
} RgbTriple;

// --------------------------------------------
// BGRAQUAD - 32 bits pixel
// --------------------------------------------
typedef struct BGRAQUAD {
    unsigned char	bgraBlue;		// blue
    unsigned char	bgraGreen;		// green
    unsigned char	bgraRed;		// red
    unsigned char	bgraAlpha;		// alpha
} BgraQuad;

class TexInfo {
public:
    int mWidth = 0;
    int mHeight= 0;
    std::vector<char> mRgbaBuf;

public:
    TexInfo() = default;
    TexInfo(const TexInfo& ti) = default;
    TexInfo(TexInfo&& ti) = default;
    TexInfo& operator= ( const TexInfo &ti) = default;
    TexInfo& operator= ( TexInfo &&ti) = default;
};

TexInfo LoadTexture(FileFormat ffmt, std::istringstream &istringstream);

#endif //CPPMD2VIEWER_TEXOBJ_H
