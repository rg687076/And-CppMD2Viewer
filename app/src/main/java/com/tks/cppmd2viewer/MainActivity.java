package com.tks.cppmd2viewer;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.widget.TextView;

import com.tks.cppmd2viewer.databinding.ActivityMainBinding;

import java.util.ArrayList;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {
    private ScaleGestureDetector mPinchDetector;
    private ArrayList<String> mDrwModelNames = new ArrayList<>();

    // Used to load the 'cppmd2viewer' library on application startup.
    static {
        System.loadLibrary("cppmd2viewer");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        GLSurfaceView glview = binding.glview;
        glview.setEGLContextClientVersion(2);
        glview.setZOrderOnTop(true);
        glview.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        glview.setEGLConfigChooser(8,8,8,8,16,8);
        glview.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

            }

            @Override
            public void onSurfaceChanged(GL10 gl10, int i, int i1) {

            }

            @Override
            public void onDrawFrame(GL10 gl10) {

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
//                Jni.setScale(mScale);
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
//                Jni.setTouchAngle(mTouchAngleX, mTouchAngleY);
                mLastX = event.getX();
                mLastY = event.getY();
            }
            return true;
        }
        return super.onTouchEvent(event);
    }

    /**
     * A native method that is implemented by the 'cppmd2viewer' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}