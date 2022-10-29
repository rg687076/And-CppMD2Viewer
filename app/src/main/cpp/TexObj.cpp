#include <sstream>
#include <android/log.h>
#include "TexObj.h"

TexInfo LoadTexture(FileFormat ffmt, std::istringstream &istringstream) {
    TexInfo retTexInfo;

    istringstream.seekg(0, std::ios::beg );
    /* BMPフォーマット */
    if(ffmt == FileFormat::BMP) {
        int bitdepth = 0;
        BmpFileHeader fileHeader = {0};
        istringstream.read((char*)&fileHeader, sizeof(BmpFileHeader));
        /* 'B','M'はじまりのチェックは完了している */

        /* ヘッダサイズ(40:Windows Bitmapのサイズ or 12:OS/2 Bitmapのサイズ) */
        int headersize = 0;
        istringstream.read((char*)&headersize, sizeof(int));
        if(headersize != 40 && headersize != 12) {
            /* フォーマット不正 : 40:Windows Bitmapのサイズ,12:OS/2 Bitmapのサイズのどちらでもない */
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed format!! offset2bin=%d(not equal 40 or 12)\n", headersize);
            return retTexInfo;
        }
        else if(headersize == 40) {
            /* Windows Bitmap(40byte) */
            BitmapInfoHeader bmpinfoheader = {.headersize=headersize};
            istringstream.read((char*)&(bmpinfoheader.width), sizeof(BitmapInfoHeader)-sizeof(int));
            retTexInfo.mWidth = bmpinfoheader.width;
            retTexInfo.mHeight= bmpinfoheader.height;
            bitdepth = bmpinfoheader.bitdepth;
            if(bmpinfoheader.comptype != 0 || bitdepth !=24) {
                /* 無圧縮しかサポートしない, 24bitカラーしか対応しない */
                __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Not Supported comptype=%d bitdepth=%d\n", bmpinfoheader.comptype, bitdepth);
                retTexInfo.mWidth = 0;
                retTexInfo.mHeight= 0;
                return retTexInfo;
            }
        }
        else if(headersize == 12) {
            /* OS/2 Bitmap(12byte) */
            BitmapCoreHeader bitmapcoreheader = {.headersize=headersize};
            istringstream.read((char*)&(bitmapcoreheader.width), sizeof(BitmapCoreHeader)-sizeof(int));
            retTexInfo.mWidth = bitmapcoreheader.width;
            retTexInfo.mHeight= bitmapcoreheader.height;
            bitdepth = bitmapcoreheader.bitdepth;
            if(bitdepth !=24) {
                /* 24bitカラーしか対応しない */
                __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Not Supported bitdepth=%d\n", bitdepth);
                retTexInfo.mWidth = 0;
                retTexInfo.mHeight= 0;
                return retTexInfo;
            }
        }

        /* BGRデータまで移動 */
        istringstream.seekg(fileHeader.offset2bin, std::ios::beg);
        /* BGRデータ一括取得 */
        int imagesize3 = retTexInfo.mWidth*retTexInfo.mHeight*3;
        char* buf = (char*)malloc(imagesize3);
        istringstream.read(buf, imagesize3);

        /* RGBAデータに変換 */
        int imagesize4 = retTexInfo.mWidth*retTexInfo.mHeight*4;
        retTexInfo.mRgbaBuf.resize(imagesize4);
        for(int rgba = 0, bgr = imagesize3-3;  rgba < imagesize4; rgba+=4,bgr-=3) {
            retTexInfo.mRgbaBuf[rgba + 0] = buf[bgr + 2];
            retTexInfo.mRgbaBuf[rgba + 1] = buf[bgr + 1];
            retTexInfo.mRgbaBuf[rgba + 2] = buf[bgr + 0];
            retTexInfo.mRgbaBuf[rgba + 3] = 0xff;
        }
        free(buf);
        return retTexInfo;
    }
    /* TGAフォーマット */
    else if(ffmt == FileFormat::TGA) {
        /* ヘッダ読込 */
        TgaHeader tgaheader;
        istringstream.read((char*)&tgaheader, sizeof(TgaHeader));

        /* 画像幅/高さ取得 */
        retTexInfo.mWidth = tgaheader.is_width;
        retTexInfo.mHeight= tgaheader.is_height;

        /* RGBA領域確保 */
        retTexInfo.mRgbaBuf.resize(tgaheader.is_width * tgaheader.is_height * 4);


    }

    return retTexInfo;
}
