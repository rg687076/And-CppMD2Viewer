package com.tks.cppmd2viewer;

import android.content.res.AssetManager;

public class Jni {
    static {
        System.loadLibrary("jni");
    }

    public static native void onCreate(AssetManager assets, String[] object);
    public static native void onSurfaceCreated();
    public static native void onSurfaceChanged(int w, int h);
    public static native void onDrawFrame();
    public static native void onDestroy();
}
