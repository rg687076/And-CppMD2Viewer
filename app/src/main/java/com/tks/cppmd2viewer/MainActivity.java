package com.tks.cppmd2viewer;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

import com.google.android.material.snackbar.Snackbar;
import com.tks.cppmd2viewer.databinding.ActivityMainBinding;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {
    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        /* MD2用GLSurfaceViewの準備 */
        GLSurfaceView glview = binding.md2glview;
        glview.setEGLContextClientVersion(2);
        glview.setZOrderOnTop(true);
        glview.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        glview.setEGLConfigChooser(8,8,8,8,16,8);
        glview.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
                Jni.onSurfaceCreated();
            }

            @Override
            public void onSurfaceChanged(GL10 gl10, int w, int h) {
                Jni.onSurfaceChanged(w, h);
            }

            @Override
            public void onDrawFrame(GL10 gl10) {
                Jni.onDrawFrame();
            }
        });

        /* Asset配下のファイル一覧を取得 */
        AtomicReference<List<String>> assetfilelist = new AtomicReference<List<String>>();
        assetfilelist.set(new ArrayList<String>());
        try {
            String[] files = MainActivity.this.getAssets().list("");
            Arrays.stream(files).forEach(f -> {
                if(f.equals("images")) return;
                if(f.equals("webkit")) return;
                List<String> filelist = assetfilelist.get();
                filelist.addAll(getFiles(MainActivity.this.getAssets(), f));
                assetfilelist.set(filelist);
            });
        }
        catch (IOException e) {
            Snackbar.make(binding.getRoot(), "初期化に失敗しました!!", Snackbar.LENGTH_SHORT).show();
            new Handler().postDelayed(() -> MainActivity.this.finish(), 10000);
        }

        Log.d("aaaaa", "file数=" + assetfilelist.get().size());
        for(String filename : assetfilelist.get().toArray(new String[0])) {
            Log.d("aaaaa", "filename="+ filename);
        }

        /* cpp側 初期化 */
        Jni.onCreate(getResources().getAssets(), assetfilelist.get().toArray(new String[0]));
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Jni.onDestroy();
    }

    private static List<String> getFiles( android.content.res.AssetManager am, String file) {
        List<String> retList = new ArrayList<String>();
        try {
            String[] subfiles = am.list(file);
            if(subfiles.length != 0) {
                Arrays.stream(subfiles).forEach(f -> {
                    retList.addAll(getFiles(am, file+"/"+f));
                });
            }
            else {
                retList.add(file);
                return retList;
            }
        }
        catch (IOException e) { return new ArrayList<String>();}
        return retList;
    }
}