package com.contactlist;

import android.os.Bundle;
import android.util.Log;

import org.qtproject.qt.android.bindings.QtActivity;

public class MainActivity extends QtActivity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("TAG", "onCreate: ");


    }
}