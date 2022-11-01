package com.tks.cppmd2viewer;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

import com.google.android.material.snackbar.Snackbar;
import com.tks.cppmd2viewer.databinding.ActivityMainBinding;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {
    private ActivityMainBinding binding;
    private ArrayList<String> mDrwModel = new ArrayList<>();

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
    }

    @Override
    protected void onStart() {
        super.onStart();

        /* モデルindexs */
        HashMap<String, ModelIndex> md2modelindex = new HashMap<>();

        /* モデルindexファイルs(model-index.json)を取得 */
        try {
            /* indexファイル読込み */
            InputStream fileInputStream = getAssets().open("model-index.json");
            byte[] readBytes = new byte[fileInputStream.available()];
            fileInputStream.read(readBytes);
            String readString = new String(readBytes);
            fileInputStream.close();
            Log.i("index-content:", readString);
            /* jsonパース */
            JSONObject jsonObject = new JSONObject(readString);
            /* jsonパース(md2models) */
            JSONArray jsonarray = jsonObject.getJSONArray("md2models");
            for(int lpct = 0; lpct < jsonarray.length(); lpct++) {
                JSONObject md2model = jsonarray.getJSONObject(lpct);
                ModelIndex mi = new ModelIndex() {{ modelname=md2model.getString("name");
                                                    md2filename =md2model.getString("vertex");
                                                    texfilename=md2model.getString("tex");
                                                    vshfilename=md2model.getString("vsh");
                                                    fshfilename=md2model.getString("fsh");}};
                mDrwModel.add(mi.modelname);
                md2modelindex.put(md2model.getString("name"), mi);
            }
        }
        catch(IOException | JSONException e) {
            e.printStackTrace();
            Snackbar.make(binding.getRoot(), "初期化に失敗しました!!", Snackbar.LENGTH_SHORT).show();
            new Handler().postDelayed(() -> MainActivity.this.finish(), 10000);
        }

        Log.d("aaaaa", "model数=" + md2modelindex.size());
        for (Map.Entry<String, ModelIndex> item : md2modelindex.entrySet())
            System.out.println(item.getKey() + " => " + item.getValue().md2filename + " : " + item.getValue().texfilename + " : " + item.getValue().vshfilename + " : " + item.getValue().fshfilename);

        /* cpp側 初期化 */
        String[] modelnames   = new String[md2modelindex.size()];
        String[] md2filenames = new String[md2modelindex.size()];
        String[] texfilenames = new String[md2modelindex.size()];
        String[] vshfilenames = new String[md2modelindex.size()];
        String[] fshfilenames = new String[md2modelindex.size()];
        int lpct = 0;
        for(Map.Entry<String, ModelIndex> item : md2modelindex.entrySet()) {
            modelnames  [lpct] = item.getKey();
            md2filenames[lpct] = item.getValue().md2filename;
            texfilenames[lpct] = item.getValue().texfilename;
            vshfilenames[lpct] = item.getValue().vshfilename;
            fshfilenames[lpct] = item.getValue().fshfilename;
        }
        Jni.onStart(getResources().getAssets(), modelnames, md2filenames, texfilenames, vshfilenames, fshfilenames);
    }

    @Override
    protected void onStop() {
        super.onStop();
        Jni.onStop();
    }
}

class ModelIndex {
    public String modelname;
    public String md2filename;
    public String texfilename;
    public String vshfilename;
    public String fshfilename;
}
