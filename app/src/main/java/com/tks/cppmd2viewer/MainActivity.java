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
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
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

        /* mqoファイル一覧(model-index.json)を取得 */
        HashMap<String, ModelIndex> modelindex = new HashMap<String, ModelIndex>();

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
            JSONArray jsonarray = jsonObject.getJSONArray("md2models");
            for(int lpct = 0; lpct < jsonarray.length(); lpct++) {
                JSONObject md2model = jsonarray.getJSONObject(lpct);
                ModelIndex mi = new ModelIndex() {{ modelname=md2model.getString("name");
                                                    vertexfilename=md2model.getString("vertex");
                                                    texfilename=md2model.getString("tex");}};
                mDrwModel.add(mi.modelname);
                modelindex.put(md2model.getString("name"), mi);
            }
        }
        catch(IOException | JSONException e) {
            e.printStackTrace();
            Snackbar.make(binding.getRoot(), "初期化に失敗しました!!", Snackbar.LENGTH_SHORT).show();
            new Handler().postDelayed(() -> MainActivity.this.finish(), 10000);
        }

        Log.d("aaaaa", "model数=" + modelindex.size());
        for (Map.Entry<String, ModelIndex> item : modelindex.entrySet())
            System.out.println(item.getKey() + " => " + item.getValue().vertexfilename + " : " + item.getValue().texfilename);

        /* cpp側 初期化 */
        String[] modelnames = modelindex.keySet().toArray(new String[0]);
        String[] vertexnames= modelindex.values().stream().map(mi -> { return mi.vertexfilename;}).toArray(String[]::new);
        String[] texnames   = modelindex.values().stream().map(mi -> { return mi.texfilename;}).toArray(String[]::new);
        Jni.onCreate(getResources().getAssets(), modelnames, vertexnames, texnames);
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

class ModelIndex {
    public String modelname;
    public String vertexfilename;
    public String texfilename;
}
