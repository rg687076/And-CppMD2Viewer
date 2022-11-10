package com.tks.cppmd2viewer.modelformat;

public class MQO {
    static {
        System.loadLibrary("cg3dviewer");
    }

    public static native boolean init();
    public static native void setDrawArea(int width, int height);
    public static native void draw();
    public static native void setTouchAngle(float aTouchAngleX, float aTouchAngleY);
    public static native void setScale(float mScale);
}
