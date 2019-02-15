/*
 * Copyright (C) 2016. Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
 * Authors:
 * Zhao Dongshuang <zhaodongshuang@syberos.com>
 *
 * This software, including documentation, is protected by copyright controlled
 * by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
 */
package com.star.demo.service;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.util.Objects;
import android.util.Log;
/**
 * Created by zhaodongshuang on 16-8-9.
 */
public class Storage extends Application {

    /////////////////////////////// private //////////////////////////////////
    private static final String TAG = "UNIFYID_Storage";

    private static final String PREFERENCES_NAME = "PREFERENCES_NAME_OTA";

    private static final String KEY_PRODUCT = "KEY_PRODUCT_OTA";
    private static final String KEY_URL = "KEY_URL_OTA";
    private static final String KEY_NOTE = "KEY_NOTE_OTA";
    private static final String KEY_VERSION = "KEY_VERSION_OTA";
    private static final String KEY_MD5 = "KEY_MD5_OTA";
    private static final String KEY_SIZE = "KEY_SIZE_OTA";
    private static final String KEY_TIME = "KEY_TIME_OTA";
    private static final String KEY_STATUS = "KEY_STATUS_OTA";
    private static final String KEY_BATTERY_STATUS = "KEY_BATTERY_STATUS_OTA";

    private static final String KEY_ALLOWED_MOBILE = "KEY_ALLOWED_MOBILE_OTA";
    private static final String KEY_DOWNLOADED_FLAG = "KEY_DOWNLOADED_FLAG_OTA";
    private static final String KEY_IN_FOREGROUND = "KEY_IN_FOREGROUND_OTA";
    private static final String KEY_DOWNLOAD_CONTINUE = "KEY_DOWNLOAD_CONTINUE_OTA";
    private static final String KEY_MD5_COMPARED = "KEY_MD5_COMPARED_OTA";
    private static final String KEY_CURRENT_VERSION = "KEY_CURRENT_VERSION_OTA";
    private static final String KEY_HAS_UPDATE = "KEY_HAS_UPDATE_OTA";
    private static final String KEY_DOWNLOAD_FILE_NOT_FOUND = "KEY_DOWNLOAD_FILE_NOT_FOUND_OTA";
    private static final String KEY_INTERVAL = "KEY_INTERVAL_OTA";

    private static final long DayMillis = 1000 * 60 * 60 * 24;
    private static final long DefaultInterval = DayMillis * 2;

    private static final String JSON_UPDATE = "update";
    private static final String JSON_PRODUCT = "product";
    private static final String JSON_URL = "url";
    private static final String JSON_NOTE = "note";
    private static final String JSON_VERSION = "version";
    private static final String JSON_MD5 = "md5";
    private static final String JSON_SIZE = "size";
    private static final String JSON_TIME = "time";

    private static Context context;
    private static SharedPreferences preferences;

    @Override
    public void onCreate() {
        super.onCreate();
        context = getApplicationContext();
        preferences = context.getSharedPreferences(PREFERENCES_NAME, Context.MODE_PRIVATE);
    }

    public static Context Context() {
        return context;
    }

    /////////////////////////////// private //////////////////////////////////

    /////////////////////////////// public ///////////////////////////////////
//    public static boolean receiveInfo(String json) {
//        Log.d(TAG, "receiveInfo: " + json);
//        try {
//            JSONObject object = (JSONObject) new JSONTokener(json).nextValue();
//            boolean hasUpdate = object.has(JSON_UPDATE) && object.optBoolean(JSON_UPDATE);
//            if (hasUpdate) {
//                preferences.edit().putString(KEY_PRODUCT, object.optString(JSON_PRODUCT)).apply();
//                String url = object.optString(JSON_URL);
////                url = url.replace("www.zoomte.cn", "thetang.f3322.net");
//                if (!Objects.equals(url, url())) {
//                    Utils.deleteUpdateFile();
//                }
//                preferences.edit().putString(KEY_URL, url).apply();
//                preferences.edit().putString(KEY_NOTE, object.optString(JSON_NOTE)).apply();
//                preferences.edit().putString(KEY_VERSION, object.optString(JSON_VERSION)).apply();
//                String md5 = object.optString(JSON_MD5);
//                if (!Objects.equals(md5, md5())) {
//                    Utils.deleteUpdateFile();
//                }
//                preferences.edit().putString(KEY_MD5, md5).apply();
//                long size = Long.parseLong(object.optString(JSON_SIZE));
//                if (!Objects.equals(size, size())) {
//                    Utils.deleteUpdateFile();
//                }
//                preferences.edit().putLong(KEY_SIZE, size).apply();
//                preferences.edit().putString(KEY_TIME, object.optString(JSON_TIME)).apply();
//            } else {
//                preferences.edit().putString(KEY_TIME, object.optString(JSON_TIME)).apply();
//            }
//            setHasUpdate(hasUpdate);
//            return hasUpdate;
//        } catch (JSONException e) {
//            Log.d(TAG, "receiveInfo Error JSONException: " + e);
//        }
//        return false;
//    }

    public static boolean hasUpdate() {
        return preferences.getBoolean(KEY_HAS_UPDATE, false);
    }

    public static void setHasUpdate(boolean hasUpdate) {
        preferences.edit().putBoolean(KEY_HAS_UPDATE, hasUpdate).apply();
    }

    public static String url() {
        return preferences.getString(KEY_URL, "");
    }

    public static String note() {
        return preferences.getString(KEY_NOTE, "");
    }

    public static String updateVersion() {
        return preferences.getString(KEY_VERSION, "");
    }

    public static String md5() {
        return preferences.getString(KEY_MD5, "");
    }

    public static long size() {
        return preferences.getLong(KEY_SIZE, -1);
    }

    public static int getStatus() {
        int s = preferences.getInt(KEY_STATUS, Utils.NONE);
        return s;
    }

    public static void setStatus(final int status) {
        String statusName;
        switch (status) {
            case Utils.CHECKING:
                statusName = "CHECKING";
                break;
            case Utils.CHECKED:
                statusName = "CHECKED";
                break;
            case Utils.DOWNLOADING:
                statusName = "DOWNLOADING";
                break;
            case Utils.DOWNLOAD_PAUSE:
                statusName = "DOWNLOAD_PAUSE";
                break;
            case Utils.DOWNLOADED:
                statusName = "DOWNLOADED";
                break;
            case Utils.VERIFY:
                statusName = "VERIFY";
                break;
            case Utils.VERIFY_FAILED:
                statusName = "VERIFY_FAILED";
                break;
            case Utils.UPGRADE:
                statusName = "UPGRADE";
                break;
            default:
                statusName = "NONE";
                break;
        }
        Log.d(TAG, "setStatus: " + statusName);

        preferences.edit().putInt(KEY_STATUS, status).commit();
    }

    public static long interval() {
        return preferences.getLong(KEY_INTERVAL, DefaultInterval);
    }

    public static int batteryStatus() {
        if (Config.batteryStatus() == Utils.NORMAL) {
            return preferences.getInt(KEY_BATTERY_STATUS, Utils.NORMAL);
        }
        return Config.batteryStatus();
    }

    public static void setBatteryStatus(int status) {
        Log.d(TAG, "setBatteryStatus: " + status);
        preferences.edit().putInt(KEY_BATTERY_STATUS, status).apply();
    }

    public static boolean allowedMobile() {
        return !Utils.NotificationEnabled || preferences.getBoolean(KEY_ALLOWED_MOBILE, false);
    }

    public static void setAllowedMobile(boolean allowed) {
        Log.d(TAG, "setAllowedMobile: " + allowed);
        preferences.edit().putBoolean(KEY_ALLOWED_MOBILE, allowed).commit();
    }

    public static boolean downloadedFlag() {
        return preferences.getBoolean(KEY_DOWNLOADED_FLAG, false);
    }

    public static void setDownloadedFlag(boolean flag) { // tip update after unlock screen
        Log.d(TAG, "setDownloadedFlag: " + flag);
        preferences.edit().putBoolean(KEY_DOWNLOADED_FLAG, flag).apply();
    }

    public static boolean inForeground() {
        return preferences.getBoolean(KEY_IN_FOREGROUND, false);
    }

    public static void setInForeground(boolean in) {
        Log.d(TAG, "setInForeground: " + in);
        preferences.edit().putBoolean(KEY_IN_FOREGROUND, in).commit();
        if (!in) {
            Intent intent = new Intent("syberos.systemupdate.action.IN_BACKGROUND");
            context.sendBroadcast(intent);
        }
    }

    public static boolean downloadContinue() {
        return preferences.getBoolean(KEY_DOWNLOAD_CONTINUE, false);
    }

    public static void setDownloadContinue(boolean ok) {
        Log.d(TAG, "setDownloadContinue: " + ok);
        preferences.edit().putBoolean(KEY_DOWNLOAD_CONTINUE, ok).commit();
    }

    public static void clear() {
        setDownloadContinue(false);
    }

    public static void saveMd5Result(String serverMd5, String currentMd5) {
        Log.d(TAG, "serverMd5: " + serverMd5 + " currentMd5: " + currentMd5);
        preferences.edit().putString(KEY_MD5_COMPARED,
                "serverMd5: " + serverMd5 + ",currentMd5: " + currentMd5).apply();
    }

    public static void saveDownloadErrorInfo(String errorInfo) {
        preferences.edit().putString(String.valueOf(System.currentTimeMillis()),
                errorInfo).apply();
    }

    public static String currentVersion() {
        return preferences.getString(KEY_CURRENT_VERSION, "");
    }

    public static void setCurrentVersion(String version) {
        preferences.edit().putString(KEY_CURRENT_VERSION, version).apply();
    }

    public static boolean downloadFileNotFound() {
        return preferences.getBoolean(KEY_DOWNLOAD_FILE_NOT_FOUND, false);
    }

    public static void setDownloadFileNotFound(boolean notFound) {
        Log.d(TAG, "setDownloadFileNotFound: " + notFound);
        preferences.edit().putBoolean(KEY_DOWNLOAD_FILE_NOT_FOUND, notFound).apply();
    }

}
