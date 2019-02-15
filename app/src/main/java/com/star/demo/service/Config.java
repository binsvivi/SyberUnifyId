/*
 * Copyright (C) 2016. Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
 * Authors:
 * Zhao Dongshuang <zhaodongshuang@syberos.com>
 *
 * This software, including documentation, is protected by copyright controlled
 * by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
 */

//package com.syberos.systemupdate;
package com.star.demo.service;
import android.content.Context;
import android.content.SharedPreferences;

/**
 * Created by zhaodongshuang on 17-3-1.
 */

class Config {
    private static final String PREFERENCES_NAME = "CONFIGURATION_UNIFYID";
    private static SharedPreferences preferences
            = Storage.Context().getSharedPreferences(PREFERENCES_NAME, Context.MODE_PRIVATE);

    private static final String KEY_CONFIG_UDID = "KEY_CONFIG_UDID_OTA";
    private static final String KEY_CONFIG_VERSION = "KEY_CONFIG_VERSION_OTA";
    private static final String KEY_CONFIG_UNIFYID_KEY = "KEY_CONFIG_UNIFYID_KEY";
    private static final String KEY_CONFIG_PRODUCT = "KEY_CONFIG_PRODUCT_OTA";
    private static final String KEY_CONFIG_CHECK_UPDATE_URL = "KEY_CONFIG_CHECK_UPDATE_URL_OTA";
    private static final String KEY_CONFIG_UPDATE_PATH = "KEY_CONFIG_UPDATE_PATH_OTA";
    private static final String KEY_CONFIG_UPDATE_FILE_NAME = "KEY_CONFIG_UPDATE_FILE_NAME_OTA";
    private static final String KEY_CONFIG_TEST_BUTTON_VISIBLE = "KEY_CONFIG_TEST_BUTTON_VISIBLE_OTA";
    private static final String KEY_CONFIG_VERSION_SUFFIX_VISIBLE = "KEY_CONFIG_VERSION_SUFFIX_VISIBLE_OTA";
    private static final String KEY_CONFIG_DOWNLOAD_URL = "KEY_CONFIG_DOWNLOAD_URL_OTA";
    private static final String KEY_CONFIG_BATTERY_STATUS = "KEY_CONFIG_BATTERY_STATUS_OTA";

    static String Udid() {
       // if (Utils.isDebugType()) {
       //     return preferences.getString(KEY_CONFIG_UDID, "");
       // }
        return "";
    }

    static String configVersion() {
        if (Utils.isDebugType()) {
            return preferences.getString(KEY_CONFIG_VERSION, "");
        }
        return "";
    }

    static String unifyidKey() {
      //  if (Utils.isDebugType()) {
            return preferences.getString(KEY_CONFIG_UNIFYID_KEY, "");
      //  }
    //    return "";
    }

    static void putUnifyidKey(String key) {
        //  if (Utils.isDebugType()) {
        preferences.edit().putString(KEY_CONFIG_UNIFYID_KEY, key);
        //  }
        //    return "";
    }

    static String Product() {
        if (Utils.isDebugType()) {
            return preferences.getString(KEY_CONFIG_PRODUCT, "");
        }
        return "";
    }

    static String CheckUpdateUrl() {
        if (Utils.isDebugType()) {
            return preferences.getString(KEY_CONFIG_CHECK_UPDATE_URL, "https://www.zoomte.cn");
        }
        return "https://www.zoomte.cn";
    }

    static String UploadUrl() {
        //if (Utils.isDebugType()) {
        //    return preferences.getString(KEY_CONFIG_CHECK_UPDATE_URL, "https://www.zoomte.cn");
        //}
        return "124.192.38.8:6060/upfiles";
    }

    final private static String MiddleBranchCode = "_MM";
    final private static String MiddleBranchSavePath = "/ssota";
    final private static String NormalBranchSavePath = "/data/upgrade";

//    static String UpdatePath() {
//        if (Utils.isDebugType()) {
//            if (Utils.versionSuffix().contains(MiddleBranchCode)) {
//                return preferences.getString(KEY_CONFIG_UPDATE_PATH, MiddleBranchSavePath);
//            }
//            return preferences.getString(KEY_CONFIG_UPDATE_PATH, NormalBranchSavePath);
//        }
//        if (Utils.versionSuffix().contains(MiddleBranchCode)) {
//            return MiddleBranchSavePath;
//        }
//        return NormalBranchSavePath;
//    }

    static String UpdateFileName() {
        if (Utils.isDebugType()) {
            return preferences.getString(KEY_CONFIG_UPDATE_FILE_NAME, "update.zip");
        }
        return "update.zip";
    }

    static boolean TestButtonVisible() {
        return Utils.isDebugType() && preferences.getBoolean(KEY_CONFIG_TEST_BUTTON_VISIBLE, false);
    }

    static boolean VersionSuffixVisible() {
        return Utils.isDebugType() && preferences.getBoolean(KEY_CONFIG_VERSION_SUFFIX_VISIBLE, true);
    }

    static String DownloadUrl() {
        if (Utils.isDebugType()) {
            return preferences.getString(KEY_CONFIG_DOWNLOAD_URL, "");
        }
        return "";
    }

    static int batteryStatus() {
        if (Utils.isDebugType()) {
            return preferences.getInt(KEY_CONFIG_BATTERY_STATUS, Utils.NORMAL);
        }
        return Utils.NORMAL;
    }
}
