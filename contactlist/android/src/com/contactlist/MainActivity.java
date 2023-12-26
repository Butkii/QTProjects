package com.contactlist;

import android.content.ContentProviderOperation;
import android.database.Cursor;
import android.database.ContentObserver;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.ContactsContract;
import android.util.Log;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.Executor;
import android.content.ContentResolver;
import org.json.JSONArray;
import org.json.JSONObject;
import org.qtproject.qt.android.bindings.QtActivity;

public class MainActivity extends QtActivity {
    private List<Map<String, String>> contacts = new ArrayList<Map<String, String>>();
    native static void updateContacts(String updates, long ptr);
    private long classPointer = 0;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("TAG", "onCreate: ");
        ContactsObserver contentObserver = new ContactsObserver();
        getApplicationContext().getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, false, contentObserver);
    }

    public void fetchContacts(long ptr) {
        Executor executor = Executors.newSingleThreadExecutor();
        Handler handler = new Handler(Looper.getMainLooper());
        ContentResolver contentResolver = getContentResolver();

        if (classPointer == 0) classPointer = ptr;

        executor.execute(() -> {
            try {
                Cursor cursor = contentResolver.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
                List<Map<String, String>> newList = new ArrayList<Map<String, String>>();

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
                                            Map<String, String> contact = new HashMap<>();
                                            contact.put("name", name);
                                            contact.put("phoneNumber", phoneNumber);
                                            newList.add(contact);
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

                handler.post(() -> {
                    if (contacts.isEmpty() || contacts.size() < newList.size()) {
                        JSONArray contactsList = new JSONArray(newList);
                        Map<String, Object> update = new HashMap<>();
                        update.put("action", "add");
                        update.put("contacts", contactsList);
                        updateContacts(new JSONObject(update).toString(), classPointer);
                    } else if (contacts.size() > newList.size()) {
                        for (int i = 0; i < contacts.size(); i++) {
                            if (!newList.contains(contacts.get(i))) {
                                Map<String, Object> update = new HashMap<>();
                                update.put("action", "delete");
                                update.put("contacts", contacts.get(i));
                                updateContacts(new JSONObject(update).toString(), classPointer);
                            }
                        }
                    }
                    else {
                        for (int i = 0; i < contacts.size(); i++) {
                            if (!newList.get(i).equals(contacts.get(i))) {
                                Map<String, Object> update = new HashMap<>();
                                update.put("action", "update");
                                update.put("contacts", newList.get(i));
                                updateContacts(new JSONObject(update).toString(), classPointer);
                            }
                        }
                    }
                    contacts = newList;
                });
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        });
    }

    private class ContactsObserver extends ContentObserver {
        public ContactsObserver() {
            super(null);
        }

        @Override
        public void onChange(boolean selfChange) {
            super.onChange(selfChange);
            fetchContacts(classPointer);
        }
    }

    public void deleteContact(String phoneNumber) {
        ContentResolver cr = getContentResolver();
        Uri contactUri = Uri.withAppendedPath(ContactsContract.PhoneLookup.CONTENT_FILTER_URI, Uri.encode(phoneNumber));
        try (Cursor cur = cr.query(contactUri, null, null, null, null)) {
            if (cur.moveToFirst()) {
                do {
                    if (cur.getString(cur.getColumnIndexOrThrow(ContactsContract.PhoneLookup.NUMBER)).equalsIgnoreCase(phoneNumber)) {
                        String lookupKey = cur.getString(cur.getColumnIndexOrThrow(ContactsContract.Contacts.LOOKUP_KEY));
                        Uri uri = Uri.withAppendedPath(ContactsContract.Contacts.CONTENT_LOOKUP_URI, lookupKey);
                        cr.delete(uri, null, null);
                        Map<String, String> del = new HashMap<>();
                        del.put(phoneNumber, cur.getString(cur.getColumnIndexOrThrow(ContactsContract.Contacts.DISPLAY_NAME)));
                        if (contacts.remove(del))
                            Log.d("TAG contacts", "deleted from contacts");
                    }
                } while (cur.moveToNext());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void addContact(String name, String number) {
        ArrayList<ContentProviderOperation> ops = new ArrayList<>();
        ops.add(ContentProviderOperation.newInsert(ContactsContract.RawContacts.CONTENT_URI)
                .withValue(ContactsContract.RawContacts.ACCOUNT_TYPE, null)
                .withValue(ContactsContract.RawContacts.ACCOUNT_NAME, null)
                .build());

        ops.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
                .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID, 0)
                .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.StructuredName.GIVEN_NAME, name)
                .build());

        ops.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
                .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID, 0)
                .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.Phone.NUMBER, number)
                .withValue(ContactsContract.CommonDataKinds.Phone.TYPE, ContactsContract.CommonDataKinds.Phone.TYPE_MOBILE)
                .build());

        try {
            getContentResolver().applyBatch(ContactsContract.AUTHORITY, ops);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void updateContactNumber(String name, String newPhoneNumber) {
        ArrayList<ContentProviderOperation> ops = new ArrayList<>();

        ops.add(ContentProviderOperation.newUpdate(ContactsContract.Data.CONTENT_URI)
                .withSelection(ContactsContract.Data.DISPLAY_NAME, new String[] { name })
                .withSelection(ContactsContract.Data.MIMETYPE, new String[] { ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE })
                .withSelection(ContactsContract.CommonDataKinds.Phone.TYPE, new String[] { String.valueOf(ContactsContract.CommonDataKinds.Phone.TYPE_MOBILE) })
                .withValue(ContactsContract.CommonDataKinds.Phone.DATA, newPhoneNumber)
                .build());

        try {
            getContentResolver().applyBatch(ContactsContract.AUTHORITY, ops);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void updateContactName(String phoneNumber, String newName) {
        ContentResolver contentResolver = getContentResolver();
        long rawContactId = 0;

        Cursor cursor = contentResolver.query(
                ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                new String[]{ContactsContract.CommonDataKinds.Phone._ID, ContactsContract.CommonDataKinds.Phone.RAW_CONTACT_ID},
                ContactsContract.CommonDataKinds.Phone.NUMBER + " = ?",
                new String[]{phoneNumber},
                null
        );

        if (cursor != null) {
            try {
                if (cursor.moveToFirst()) {
                    do {
                        rawContactId = cursor.getLong(cursor.getColumnIndexOrThrow(ContactsContract.CommonDataKinds.Phone.RAW_CONTACT_ID));
                    } while (cursor.moveToNext());
                }
            } finally {
                cursor.close();
            }
        }

        ArrayList<ContentProviderOperation> ops = new ArrayList<>();
        ops.add(ContentProviderOperation.newUpdate(ContactsContract.Data.CONTENT_URI)
                .withSelection(ContactsContract.Data.CONTACT_ID, new String[]{String.valueOf(rawContactId)})
                .withSelection(ContactsContract.Data.MIMETYPE, new String[]{ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE})
                .withValue(ContactsContract.CommonDataKinds.StructuredName.GIVEN_NAME, newName)
                .build());

        try {
            contentResolver.applyBatch(ContactsContract.AUTHORITY, ops);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}