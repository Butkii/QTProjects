package com.contactlist;

import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;

public class ContactsTask implements Callable<Map<String, String>> {

    private final ContentResolver contentResolver;

    public ContactsTask(ContentResolver contentResolver) {
        this.contentResolver = contentResolver;
    }

    @Override
    public Map<String, String> call() {
        Map<String, String> contactMap = new HashMap<>();
        Cursor cursor = contentResolver.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);

        if (cursor != null) {
            try {
                while (cursor.moveToNext()) {
                    String id = cursor.getString(cursor.getColumnIndexOrThrow(ContactsContract.Contacts._ID));
                    String name = cursor.getString(cursor.getColumnIndexOrThrow(ContactsContract.Contacts.DISPLAY_NAME));

                    if (cursor.getInt(cursor.getColumnIndexOrThrow(ContactsContract.Contacts.HAS_PHONE_NUMBER)) > 0) {
                        Cursor phoneCursor = null;
                        try {
                            phoneCursor = contentResolver.query(
                                    ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                                    null,
                                    ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = ?",
                                    new String[]{id},
                                    null);
                            if (phoneCursor != null) {
                                while (phoneCursor.moveToNext()) {
                                    String phoneNumber = phoneCursor.getString(phoneCursor.getColumnIndexOrThrow(ContactsContract.CommonDataKinds.Phone.NUMBER));
                                    // Store the contact in the map
                                    contactMap.put(name, phoneNumber);
                                }
                                phoneCursor.close();
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                cursor.close();
            }
        }
        return contactMap;
    }
}
