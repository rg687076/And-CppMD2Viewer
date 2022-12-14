package com.tks.cppmd2viewer;

import android.content.res.AssetManager;

public class Jni {
    static {
        System.loadLibrary("jni");
    }

    public static native boolean onStart(AssetManager assets, String[] modelnames, String[] md2filenames, String[] texfilenames, String[] vshfilenames, String[] fshfilenames, float[] scalex, float[] scaley, float[] scalez, float[] rotx, float[] roty, float[] rotz, float[] transx, float[] transy, float[] transz);
    public static native boolean onSurfaceCreated();
    public static native void onSurfaceChanged(int width, int height);
    public static native void onDrawFrame();
    public static native void setTouchAngle(float aTouchAngleX, float aTouchAngleY);
    public static native void setScale(float mScale);

    /* MQO */
    public static native boolean init(AssetManager assets, String[] toArray);
}
