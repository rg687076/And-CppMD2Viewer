package com.tks.cppmd2viewer;

import android.content.res.AssetManager;

public class Jni {
    static {
        System.loadLibrary("jni");
    }

    public static native boolean onStart(AssetManager assets, String[] modelnames, String[] md2filenames, String[] texfilenames, String[] vshfilenames, String[] fshfilenames);
    public static native void onSurfaceCreated();
    public static native void setModelPosition(String modelname, float x, float y, float z);
    public static native void onSurfaceChanged(int w, int h);
    public static native void onDrawFrame();
    public static native void onStop();
    public static native void setScale(float mScale);
    public static native void setRotate(float mTouchAngleX, float mTouchAngleY);
}
