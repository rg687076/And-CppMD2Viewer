//
// Created by jun on 2021/06/07.
//
#ifndef CG3DIEWER_CG3DOPENGLES2_H
#define CG3DIEWER_CG3DOPENGLES2_H

#include <map>
#include <string>
#include <array>
#include <jni.h>
#include "format/MQO.h"

class CG3DViewer {
public:
    static bool init();
    static void setDrawArea(int width, int height);
    static void draw();
    static void setTouchAngle(float x, float y);

	static void setScale(float scale);
};
#endif //MQOVIEWER_TKSOPENGLES2_H
