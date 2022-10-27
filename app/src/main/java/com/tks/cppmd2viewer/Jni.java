package com.tks.cppmd2viewer;

import android.content.res.AssetManager;

public class Jni {
    static {
        System.loadLibrary("jni");
    }

    public static native boolean onStart(AssetManager assets, String[] object, String[] vertexnames, String[] texnames);
    public static native void onSurfaceCreated();
    public static native void onSurfaceChanged(int w, int h);
    public static native void onDrawFrame();
    public static native void onStop();
}
