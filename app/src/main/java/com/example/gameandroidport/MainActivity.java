package com.example.gameandroidport;

import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;

/**
 * Main Activity для SDL3 Native приложения.
 * Расширяет NativeActivity, что позволяет загружать и запускать
 * C++ код напрямую через JNI без промежуточного Java слоя.
 */
public class MainActivity extends NativeActivity {
    private static final String TAG = "Game2d";

    static {
        try {
            // Загружаем нашу native библиотеку
            System.loadLibrary("Game2d");
            Log.i(TAG, "✅ Native library Game2d loaded successfully");
        } catch (UnsatisfiedLinkError e) {
            // Ошибка загрузки библиотеки
            Log.e(TAG, "❌ Failed to load native library: " + e.getMessage(), e);
            // Re-throw to let system handle it
            throw e;
        } catch (Exception e) {
            // Другие ошибки
            Log.e(TAG, "❌ Unexpected error loading library: " + e.getMessage(), e);
            throw new RuntimeException(e);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "onCreate called");
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate completed");
        // Остальная инициализация происходит в C++ коде
    }

    @Override
    protected void onStart() {
        Log.i(TAG, "onStart called");
        super.onStart();
    }

    @Override
    protected void onResume() {
        Log.i(TAG, "onResume called");
        super.onResume();
    }

    @Override
    protected void onPause() {
        Log.i(TAG, "onPause called");
        super.onPause();
    }

    @Override
    protected void onStop() {
        Log.i(TAG, "onStop called");
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        Log.i(TAG, "onDestroy called");
        super.onDestroy();
    }
}
