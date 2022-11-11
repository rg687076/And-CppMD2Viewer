package com.tks.cppmd2viewer;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.widget.TextView;

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
import java.util.concurrent.atomic.AtomicReference;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {
    private ScaleGestureDetector mPinchDetector;
    private ArrayList<String> mDrwModelNames = new ArrayList<>();

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        GLSurfaceView glview = binding.glview;
        glview.setEGLContextClientVersion(2);
        glview.setZOrderOnTop(true);
        glview.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        glview.setEGLConfigChooser(8,8,8,8,16,8);
        glview.setBackgroundColor(Color.argb(120, 178, 63, 0));
        glview.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
                /* ***************** TODO 削除予定 ここから */
                /* Asset配下のファイル一覧を取得 */
                AtomicReference<List<String>> atomfilelist = new AtomicReference<List<String>>();
                atomfilelist.set(new ArrayList<String>());
                try {
                    String[] files = MainActivity.this.getAssets().list("");
                    Arrays.stream(files).forEach(f -> {
                        if(f.equals("images")) return;
                        if(f.equals("webkit")) return;
                        if(f.equals("female.md2")) return;
                        if(f.equals("female.tga")) return;
                        if(f.equals("grunt.md2")) return;
                        if(f.equals("grunt.tga")) return;
                        if(f.equals("model-index.json")) return;
                        List<String> filelist = atomfilelist.get();
                        List<String> tmpaddfiles = getFiles(MainActivity.this.getAssets(), f);
                        tmpaddfiles.remove("shaders/basic.fsh");
                        tmpaddfiles.remove("shaders/basic.vsh");
                        filelist.addAll(tmpaddfiles);
                        atomfilelist.set(filelist);
                    });

                }
                catch (IOException e) { }

                boolean ret = Jni.init(MainActivity.this.getAssets(), atomfilelist.get().toArray(new String[atomfilelist.get().size()]));
                if(!ret) throw new RuntimeException("CG3DViewer.init()で失敗");
                /* ***************** 削除予定 ここまで */
                boolean ret1 = Jni.onSurfaceCreated();
                if(!ret1) throw new RuntimeException("Jni.init()で失敗");
            }

            @Override
            public void onSurfaceChanged(GL10 gl10, int width, int height) {
                mHeight = height;
                Jni.onSurfaceChanged(width, height);
            }

            @Override
            public void onDrawFrame(GL10 gl10) {
                Jni.onDrawFrame();
            }
        });

        mPinchDetector = new ScaleGestureDetector(this.getApplicationContext(), new ScaleGestureDetector.SimpleOnScaleGestureListener() {
            @Override
            public boolean onScaleBegin(ScaleGestureDetector detector) {
                return super.onScaleBegin(detector);
            }

            @Override
            public boolean onScale(ScaleGestureDetector detector) {
                mScale = detector.getScaleFactor();
                if(mScale > 5) mScale = 5;
                else if(mScale < 0.2) mScale = 0.2f;
                Jni.setScale(mScale);
                return super.onScale(detector);
            }

            @Override
            public void onScaleEnd(ScaleGestureDetector detector) {
                super.onScaleEnd(detector);
            }
        });
    }

    private float mScale = 1.0f;
    private float mTouchAngleX, mTouchAngleY;
    private float mLastX,mLastY;
    private int mHeight;
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if(event.getPointerCount() == 2)
            return mPinchDetector.onTouchEvent(event);

        switch(event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                mLastX = event.getX();
                mLastY = event.getY();
                return true;
            case MotionEvent.ACTION_MOVE:
            {
                float factor = 100f / mHeight;
                float dx = factor * (event.getX() - mLastX);
                float dy = factor * (event.getY() - mLastY);
                mTouchAngleX = Math.max(Math.min(mTouchAngleX+dy,90f),-90f);
                mTouchAngleY += dx;
                Jni.setTouchAngle(mTouchAngleX, mTouchAngleY);
                mLastX = event.getX();
                mLastY = event.getY();
            }
            return true;
        }
        return super.onTouchEvent(event);
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
                ModelIndex mi = new ModelIndex() {{
                    modelname=md2model.getString("name");
                    md2filename=md2model.getString("ver");
                    texfilename=md2model.getString("tex");
                    vshfilename=md2model.getString("vsh");
                    fshfilename=md2model.getString("fsh");}};
                mDrwModelNames.add(mi.modelname);
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
            lpct++;
        }
        Jni.onStart(getResources().getAssets(), modelnames, md2filenames, texfilenames, vshfilenames, fshfilenames);
    }

    @Override
    protected void onStop() {
        super.onStop();
    }
}

class ModelIndex {
    public String modelname;
    public String md2filename;
    public String texfilename;
    public String vshfilename;
    public String fshfilename;
}
