package com.contactlist;

import android.content.ContentProviderOperation;
import android.content.ContentProviderResult;
import android.content.Intent;
import android.content.OperationApplicationException;
import android.database.Cursor;
import android.database.ContentObserver;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.RemoteException;
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
import org.json.JSONException;
import org.json.JSONObject;
import org.qtproject.qt.android.bindings.QtActivity;

public class MainActivity extends QtActivity {
    private final Executor executor = Executors.newSingleThreadExecutor();
    private final Handler handler = new Handler(Looper.getMainLooper());
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
        Log.d("MYINT", "value: " + ptr);
        if (classPointer == 0) classPointer = ptr;
        ContentResolver contentResolver = getContentResolver();
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
                                            contact.put(phoneNumber, name);
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
                    Log.d("TAG contacts current", newList.toString());
                    Log.d("TAG contacts current", contacts.toString());

                    if (contacts.isEmpty()) {
                        Log.d("TAG Contacts", "empty");
                        JSONArray contactsList = new JSONArray(newList);
                        Map<String, Object> update = new HashMap<>();
                        update.put("action", "add");
                        update.put("contacts", contactsList);
                        JSONObject json = new JSONObject(update);
                        Log.d("TAG contactslist", json.toString());
                        updateContacts(json.toString(), classPointer);
                    } else {
                        if (contacts.size() < newList.size()) {
                            Log.d("TAG Contacts", "added");
                            JSONArray contactsList = new JSONArray(newList);
                            Map<String, Object> update = new HashMap<>();
                            update.put("action", "add");
                            update.put("contacts", contactsList);
                            JSONObject json = new JSONObject(update);
                            updateContacts(json.toString(), classPointer);
                        }
                        else if (contacts.size() > newList.size()) {
                            Log.d("TAG Contacts", "deleted");
                            for (int i = 0; i < contacts.size(); i++) {
                                if (!newList.contains(contacts.get(i))) {
                                    Map<String, Object> update = new HashMap<>();
                                    update.put("action", "delete");
                                    update.put("contacts", contacts.get(i));
                                    JSONObject json = new JSONObject(update);
                                    updateContacts(json.toString(), classPointer);
                                }
                            }
                        }
                        else {
                            Log.d("TAG Contacts", "updated");
                            for (int i = 0; i < contacts.size(); i++) {
                                if (!newList.get(i).equals(contacts.get(i))) {
                                    Log.d("TAG Contacts", newList.get(i).toString() + contacts.get(i).toString());
                                    Map<String, Object> update = new HashMap<>();
                                    update.put("action", "update");
                                    update.put("contacts", newList.get(i));
                                    JSONObject json = new JSONObject(update);
                                    updateContacts(json.toString(), classPointer);
                                }
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
            Log.d("TAG contacts", "Contacts changed");
            fetchContacts(classPointer);
        }
    }

    public void deleteContact(String phoneNumber) {
        Log.d("TAG", phoneNumber);
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
        ArrayList<ContentProviderOperation> op_list = new ArrayList<ContentProviderOperation>();
        op_list.add(ContentProviderOperation.newInsert(ContactsContract.RawContacts.CONTENT_URI)
                .withValue(ContactsContract.RawContacts.ACCOUNT_TYPE, null)
                .withValue(ContactsContract.RawContacts.ACCOUNT_NAME, null)
                //.withValue(RawContacts.AGGREGATION_MODE, RawContacts.AGGREGATION_MODE_DEFAULT)
                .build());

        // first and last names
        op_list.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
                .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID, 0)
                .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.StructuredName.GIVEN_NAME, name)
                .build());

        op_list.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
                .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID, 0)
                .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.Phone.NUMBER, number)
                .withValue(ContactsContract.CommonDataKinds.Phone.TYPE, ContactsContract.CommonDataKinds.Phone.TYPE_HOME)
                .build());

        try {
            getContentResolver().applyBatch(ContactsContract.AUTHORITY, op_list);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void updateContact(String name, String newPhoneNumber) {
        Log.d("TAG contacts", "update: " + name + newPhoneNumber);
        ContentResolver contentResolver = getContentResolver();
        ArrayList<ContentProviderOperation> ops = new ArrayList<>();

        // Define the criteria for updating the contact
        String where = ContactsContract.Data.DISPLAY_NAME + " = ? AND " +
                ContactsContract.Data.MIMETYPE + " = ? AND " +
                String.valueOf(ContactsContract.CommonDataKinds.Phone.TYPE) + " = ? ";

        // Set the parameters for the selection criteria
        String[] params = new String[] {
                name,
                ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE,
                String.valueOf(ContactsContract.CommonDataKinds.Phone.TYPE_HOME)
        };

        // Create an update operation for the contact
        ops.add(ContentProviderOperation.newUpdate(ContactsContract.Data.CONTENT_URI)
                .withSelection(where, params)
                .withValue(ContactsContract.CommonDataKinds.Phone.DATA, newPhoneNumber)
                .build());

        try {
            // Apply the batch update to the Contacts Provider
            contentResolver.applyBatch(ContactsContract.AUTHORITY, ops);
        } catch (Exception e) {
            // Handle exceptions, typically caused by permission issues or other errors
            e.printStackTrace();
        }
    }
}