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
                __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Not Supported bitdepth=%d", bitdepth);
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

        /* カラーパレット読込み */
        std::unique_ptr<RgbTriple[]> pPalette = nullptr;
        if(tgaheader.color_map_type) {
            /* 24ビットイメージと32ビットイメージはパレット化されない */
            pPalette = std::make_unique<RgbTriple[]>(tgaheader.cm_length);
            istringstream.read((char*)&(pPalette[0]), sizeof(RgbTriple) * tgaheader.cm_length);
        }

        bool flipvert = true;
        unsigned char *ptr = nullptr;
        switch(tgaheader.image_type) {
            case 0: __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Not Supported ImageType=%d", tgaheader.image_type); break;  /* not supported */
            case 1: /* COLOR-MAPPED BGR 8 BITS GREYSCALE case 3の処理と同じ */
//              break;
            case 3: /* COLOR-MAPPED BGR 8 BITS */ {
                for(int rowidx = tgaheader.is_height - 1; rowidx >= 0; rowidx-- ) {
                    if( flipvert )
                        ptr = (unsigned char*)&(retTexInfo.mRgbaBuf[rowidx * tgaheader.is_width * 4 ]);

                    for(int col = 0; col < tgaheader.is_width; col++, ptr += 4 ) {
                        // read the current pixel
                        unsigned char coloridx;
                        istringstream.read((char*)&coloridx, sizeof(unsigned char));

                        // convert indexed pixel (8 bits) into rgba (32 bits) pixel
                        ptr[0] = pPalette[ coloridx ].rgbtRed;  // b->r
                        ptr[1] = pPalette[ coloridx ].rgbtGreen;// g->g
                        ptr[2] = pPalette[ coloridx ].rgbtBlue; // r->b
                        ptr[3] = 0xff;                          // alpha
                    }
                }
                break;
            }
            case 2:{
                for(int rowidx = tgaheader.is_height - 1; rowidx >= 0; rowidx-- ) {
                    if( flipvert )
                        ptr = (unsigned char*)&(retTexInfo.mRgbaBuf[rowidx * tgaheader.is_width * 4]);

                    for(int colidx = 0; colidx < tgaheader.is_width; colidx++, ptr += 4 ) {
                        switch( tgaheader.is_pixel_depth ) {
                            case 16: {  // TRUE-COLOR BGR 16 BITS
                                // read the current pixel
                                unsigned char color;
                                istringstream.read((char*)&color, sizeof(unsigned short));

                                // convert bgr (16 bits) pixel into rgba (32 bits) pixel
                                ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
                                ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
                                ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
                                ptr[3] = 255;							// alpha

                                break;
                            }
                            case 24: {  // TRUE-COLOR BGR 24 BITS
                                // convert bgr (24 bits) pixel into rgba (32 bits) pixel
                                RgbTriple pix;
                                istringstream.read((char*)&pix, sizeof(RgbTriple));

                                ptr[0] = pix.rgbtRed;
                                ptr[1] = pix.rgbtGreen;
                                ptr[2] = pix.rgbtBlue;
                                ptr[3] = 255;

                                break;
                            }
                            case 32: {  // TRUE-COLOR BGR 32 BITS
                                // convert bgr (32 bits) pixel into rgba (32 bits) pixel
                                BgraQuad pix;
                                istringstream.read((char*)&pix, sizeof(BgraQuad));

                                ptr[0] = pix.bgraRed;
                                ptr[1] = pix.bgraGreen;
                                ptr[2] = pix.bgraBlue;
                                ptr[3] = pix.bgraAlpha;

                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 9: // RLE COLOR-MAPPED BGR 8 BITS
            case 11:// RLE COLOR-MAPPED BGR 8 BITS GREYSCALE
            {
                unsigned char packetHeader, packetSize;
                for(int rowidx = tgaheader.is_height - 1; rowidx >= 0; rowidx-- ) {
                    if( flipvert )
                        ptr = (unsigned char*)&(retTexInfo.mRgbaBuf[rowidx * tgaheader.is_width * 4]);

                    for(int col = 0; col < tgaheader.is_width; /* rien */ ) {
                        istringstream.read((char*)&packetHeader, sizeof(unsigned char));
                        packetSize = 1 + (packetHeader & 0x7f);
                        if( packetHeader & 0x80 ) {
                            // run-length packet
                            // read the current pixel
                            unsigned char coloridx;
                            istringstream.read((char*)&coloridx, sizeof(unsigned char));

                            // convert indexed pixel (8 bits) pixel into rgba (32 bits) pixel
                            for(int lpi = 0; lpi < packetSize; lpi++, ptr += 4, col++ ) {
                                ptr[0] = pPalette[ coloridx ].rgbtRed;     // b->r
                                ptr[1] = pPalette[ coloridx ].rgbtGreen;   // g->g
                                ptr[2] = pPalette[ coloridx ].rgbtBlue;    // r->b
                                ptr[3] = 255;                           // alpha
                            }
                        }
                        else {
                            // non run-length packet
                            for(int lpi = 0; lpi < packetSize; lpi++, ptr += 4, col++ ) {
                                // read the current pixel
                                unsigned char coloridx;
                                istringstream.read((char*)&coloridx, sizeof(unsigned char));

                                // convert indexed pixel (8 bits) pixel into rgba (32 bits) pixel
                                ptr[0] = pPalette[ coloridx ].rgbtRed;		// b->r
                                ptr[1] = pPalette[ coloridx ].rgbtGreen;	// g->g
                                ptr[2] = pPalette[ coloridx ].rgbtBlue;		// r->b
                                ptr[3] = 255;							// alpha
                            }
                        }
                    }
                }
                break;
            }
            case 10: {
                unsigned char packetHeader, packetSize;
                for(int rowidx = tgaheader.is_height - 1; rowidx >= 0; rowidx-- ) {
                    if( flipvert )
                        ptr = (unsigned char*)&(retTexInfo.mRgbaBuf[rowidx * tgaheader.is_width * 4]);

                    for(int colidx = 0; colidx < tgaheader.is_width; /* rien */ ) {
                        istringstream.read((char*)&packetHeader, sizeof(unsigned char));
                        packetSize		= 1 + (packetHeader & 0x7f);
                        if( packetHeader & 0x80 ) {
                            // run-length packet
                            switch( tgaheader.is_pixel_depth ) {
                                case 16: {  // RLE TRUE-COLOR BGR 16 BITS
                                    // read the current pixel
                                    unsigned short color;
                                    istringstream.read((char*)&color, sizeof(unsigned short));

                                    // convert bgr (16 bits) pixel into rgba (32 bits) pixel
                                    for(int lpi = 0; lpi < packetSize; lpi++, ptr += 4, colidx++ ) {
                                        ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
                                        ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
                                        ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
                                        ptr[3] = 255;
                                    }
                                    break;
                                }

                                case 24: {  // RLE TRUE-COLOR BGR 24 BITS
                                    // convert bgr (24 bits) pixel into rgba (32 bits) pixel
                                    RgbTriple pix;
                                    istringstream.read((char*)&pix, sizeof(RgbTriple));

                                    for(int lpi = 0; lpi < packetSize; lpi++, ptr += 4, colidx++ ) {
                                        ptr[0] = pix.rgbtRed;
                                        ptr[1] = pix.rgbtGreen;
                                        ptr[2] = pix.rgbtBlue;
                                        ptr[3] = 255;
                                    }
                                    break;
                                }

                                case 32: {  // RLE TRUE-COLOR BGR 32 BITS
                                    // convert bgr (32 bits) pixel into rgba (32 bits) pixel
                                    BgraQuad pix;
                                    istringstream.read((char*)&pix, sizeof(BgraQuad));

                                    for(int lpi = 0; lpi < packetSize; lpi++, ptr += 4, colidx++ ) {
                                        ptr[0] = pix.bgraRed;
                                        ptr[1] = pix.bgraGreen;
                                        ptr[2] = pix.bgraBlue;
                                        ptr[3] = pix.bgraAlpha;
                                    }
                                    break;
                                }
                            }
                        }
                        else {
                            // non run-length packet
                            for(int lpi = 0; lpi < packetSize; lpi++, ptr += 4, colidx++ ) {
                                switch( tgaheader.is_pixel_depth ) {
                                    case 16: {  // RLE TRUE-COLOR BGR 16 BITS
                                        // read the current pixel
                                        unsigned short color;
                                        istringstream.read((char*)&color, sizeof(unsigned short));

                                        // convert bgr (16 bits) pixel into rgba (32 bits) pixel
                                        ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
                                        ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
                                        ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
                                        ptr[3] = 255;							// alpha

                                        break;
                                    }

                                    case 24: {  // RLE TRUE-COLOR BGR 24 BITS
                                        // convert bgr (24 bits) pixel into rgba (32 bits) pixel
                                        RgbTriple pix;
                                        istringstream.read((char*)&pix, sizeof(RgbTriple));

                                        ptr[0] = pix.rgbtRed;
                                        ptr[1] = pix.rgbtGreen;
                                        ptr[2] = pix.rgbtBlue;
                                        ptr[3] = 255;

                                        break;
                                    }

                                    case 32: {  // RLE TRUE-COLOR BGR 32 BITS
                                        // convert bgr (32 bits) pixel into rgba (32 bits) pixel
                                        BgraQuad pix;
                                        istringstream.read((char*)&pix, sizeof(BgraQuad));

                                        ptr[0] = pix.bgraRed;
                                        ptr[1] = pix.bgraGreen;
                                        ptr[2] = pix.bgraBlue;
                                        ptr[3] = pix.bgraAlpha;

                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            default: {
                __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Unknown format!! ImageType=%d", tgaheader.image_type);
                retTexInfo.mWidth = 0;
                retTexInfo.mHeight = 0;
                std::vector<char>().swap(retTexInfo.mRgbaBuf);
            }
        }

        pPalette.reset();
    }

    return retTexInfo;
}
