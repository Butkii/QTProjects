package com.contactlist;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.Executor;
import android.content.ContentResolver;
import org.qtproject.qt.android.bindings.QtActivity;

public class MainActivity extends QtActivity {

    static String contacts = "";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("TAG", "onCreate: ");
        ContentResolver contentResolver = getContentResolver();
        TaskRunner taskRunner = new TaskRunner();
        taskRunner.executeAsync(new ContactsTask(contentResolver), (data) -> {
            for (Map.Entry<String, String> entry : data.entrySet()) {
                String name = entry.getKey();
                String phoneNumber = entry.getValue();
                Log.d("TAG", "Contact: " + name + ", Phone Number: " + phoneNumber);
                contacts += "Contact: " + name + ", Phone Number: " + phoneNumber;
            }
            Log.d("TAG", "Contacts String: " + contacts);
        });
    }

    public static String getContactList() {
        return contacts;
    }
}