/*
 * Copyright (C) 2016. Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
 * Authors:
 * Zhao Dongshuang <zhaodongshuang@syberos.com>
 *
 * This software, including documentation, is protected by copyright controlled
 * by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
 */

package com.star.demo.service;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.os.PowerManager;
import android.os.RecoverySystem;
import android.os.StatFs;
import android.provider.Settings;
import android.service.notification.StatusBarNotification;
import android.telephony.TelephonyManager;
import android.widget.RemoteViews;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Serializable;
import java.lang.Math;
import java.math.BigDecimal;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.Objects;
import android.util.Log;
//import com.log.service.client;
//import com.syber.internal.ConfigManager;
//import com.syber.internal.ConfigValue;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

/**
 * Created by zhaodongshuang on 2016/07/02.
 * add burn function by zhaodongshuang on 2016/07/04.
 * Singleton Utils on 2016/07/18.
 */
class Utils implements Serializable {
    private static final String TAG = "UnifyId_Utils";

    private static final String TAG_NOTIFICATION = "SyberOS.SystemUpdate.Notification.Tag";

    private static final int ID_NOTIFICATION = 1901;

    static final int BATTERY_NOT_CHARGING_LOW = 30;

    static final int BATTERY_CHARGING_LOW = 10;

    /////////////////////////////// check update result ///////////////////////////////
    static final int HAS_UPDATE = 1;
    static final int NO_UPDATE = 0;
    static final int CONNECT_ERROR = -1;
    static final int CONNECT_TIME_OUT = -2;
    /////////////////////////////// check update result ///////////////////////////////

//    private static ConfigManager configManager = null;

    /**
     * Status information
     */
    static final int NONE = 0;
    static final int CHECKING = 1;
    static final int CHECKED = 2;
    static final int DOWNLOADING = 3;
    static final int DOWNLOAD_PAUSE = 4;
    static final int DOWNLOADED = 5;
    static final int VERIFY = 6;
    static final int VERIFY_FAILED = 7;
    static final int UPGRADE = 8;

    /**
     * Battery status information
     * NORMAL
     * LOW
     * CHARGING
     * CHARGING_LOW
     * UNKNOWN
     */
    static final int NORMAL = 0;
    static final int LOW = 1;
    static final int CHARGING = 2;
    static final int CHARGING_LOW = 3;

    /**
     * OTA Network Type
     * DISCONNECTED
     * WIFI
     * MOBILE
     */
    static final int DISCONNECTED = 0;
    static final int WIFI = 1;
    static final int MOBILE = 2;

    static boolean ContinueDownloading = true;
    static final int TimeoutDuration = 5000;
    static final int RetryTimes = 200;

    private static PowerManager.WakeLock wakeLock = null;
    private static final String Type_Debug = "USERDEBUG";
    private static final String IMEI_KEY = "gsm.imei1";

    static final boolean NotificationEnabled = true;

    static String address() {
        String a = Config.CheckUpdateUrl();
        Log.d(TAG, "CheckUpdateUrl: " + a);
        return a;
    }

    /**
     * get device udid information
     *
     * @return device udid string
     */
//    static String UDID() {
//        if (configManager == null) {
//            configManager = new ConfigManager();
//        }
//        ConfigValue value = new ConfigValue();
//        int r = configManager.get("com.syber.udid", value);
//        Log.d(TAG, "UDID: " + r);
//        if (0 == r) {
//            return value.getString();
//        }
//        return "";
//    }
    static String getUDID(Context context) {
        //String u = Config.Udid();
        //if (!u.isEmpty()) {
        //    return u;
        //}
    //    TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
    //    String id = tm.getDeviceId();
     //   Log.d(TAG, "getDeviceId: " + id);
      //  id = PropertyUtils.get(IMEI_KEY, id);
      //  Log.d(TAG, "PropertyUtils.get id: " + id);
//        String id = android.provider.Settings.System.getString(super.getContentResolver(),
//                android.provider.Settings.Secure.ANDROID_ID).toUpperCase();
        String androidID = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
        String id = androidID + Build.SERIAL;

        Log.d(TAG, "getUDID: " + id);
        return id;
    }

    /**
     * get device version information
     *
     * @return device version string
     */
//    static String version() {
////        String v = "";
////        if (configManager == null) {
////            configManager = new ConfigManager();
////        }
////        ConfigValue value = new ConfigValue();
////        int r = configManager.get("com.syber.version", value);
////        Log.d(TAG, "version: " + r);
////        if (0 == r) {
////            v = value.getString();
////        }
//
//        if (CurrentVersion.isEmpty()){
//            CurrentVersion = versionFromClient();
//        }
//
//        String v = CurrentVersion;
//
//        Log.d(TAG, "version from client: " + v);
//        if (v.isEmpty()) {
//            v = Build.DISPLAY;
//        }
//        return v;
//    }

    private static String CurrentVersion = "";

 //   private static String versionFromClient() {
 //       client myClient = new client(null);
 //       return myClient.getInternalVersion().trim();
 //   }

 //   static String versionSuffix() {
 //       String v = Config.configVersion();
 //       if (!v.isEmpty()) {
 //           return v;
 //       }
 //       String s = version() + (Config.VersionSuffixVisible() ? "_" + Utils.type() : "");
 //       Log.d(TAG, "versionSuffix: " + s);
 //       return s;
 //   }

    /**
     * get device product name information
     *
     * @return device product name string
     */
    static String product() {
        String p = Config.Product();
        if (!p.isEmpty()) {
            return p;
        }
        p = Build.MODEL.replace("Lenovo", "").trim();
//        if (configManager == null) {
//            configManager = new ConfigManager();
//        }
//        ConfigValue value = new ConfigValue();
//        int r = configManager.get("com.syber.product", value);
//        if (0 == r) {
//            p = value.getString();
//        }
//        Log.d(TAG, "r: " + r + "product: " + p);
        return p;
    }

    static String type() {
        return Build.TYPE.toUpperCase();
    }

    static boolean isDebugType() {
        return type().equals(Type_Debug);
    }

    /**
     * Use to get network type
     *
     * @param context
     * @return OTANetworkType an enum type, value is DISCONNECTED WIFI or MOBILE
     */
    static int networkType(Context context) {
        ConnectivityManager connectivityManager =
                (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);

        NetworkInfo info = connectivityManager.getActiveNetworkInfo();

        if (info != null && info.isConnected()) {
            if (info.getType() == ConnectivityManager.TYPE_MOBILE) {
                return MOBILE;
            } else if (info.getType() == ConnectivityManager.TYPE_WIFI) {
                return WIFI;
            }
        }
        return DISCONNECTED;
    }

    ///////////////////////////////hasEnoughMemory////////////////////////////////////
    static boolean hasEnoughMemory(long needSize) {
        Log.d(TAG, "hasEnoughMemory: " + getSystemAvailableSize());
        return getSystemAvailableSize() > needSize;
    }

    private static long getSystemAvailableSize() {
        return getAvailableSize("/data");
    }

    private static long getAvailableSize(String path) {
        StatFs fileStats = new StatFs(path);
        fileStats.restat(path);
        return fileStats.getAvailableBlocksLong() * fileStats.getBlockSizeLong();
    }
    ///////////////////////////////hasEnoughMemory////////////////////////////////////

    ///////////////////////////////formatSize////////////////////////////////////
    static String formatSize(long size) {
        if (0 >= size) {
            return "0";
        }
        return bytesFormat(size);
    }

    private static final String[] UnitNames = {"Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

    private static String bytesFormat(long bytes) {
        if (0 >= bytes) {
            return "0";
        }
        int i = (int) Math.floor(Math.log(bytes) / Math.log(1024));
        BigDecimal bd = new BigDecimal(bytes * 1.0 / Math.pow(1024, Math.floor(i)));
        bd = bd.setScale((i > 1) ? 2 : 0, BigDecimal.ROUND_HALF_UP);
        return bd.toString() + UnitNames[i];
    }
    ///////////////////////////////formatSize////////////////////////////////////

    /**
     * burn new system update package
     */
//    static void burn(Context context, String packagePath) {
//        Log.d(TAG, "burn Context: " + context.toString() + " packagePath: " + packagePath);
//
//        Storage.setStatus(NONE);
//
//        File pFile = new File(packagePath);
//
//        if (pFile.isFile()) {
//            try {
//                Storage.setAllowedMobile(false);
//                Storage.setCurrentVersion("");
//                Storage.setDownloadContinue(false);
//                Storage.setInForeground(false);
//                Storage.setHasUpdate(false);
//                // TODO: 16-8-23 send message to OTA service
//                Storage.setStatus(UPGRADE);
//
//                RecoverySystem.installPackage(context, pFile);
//            } catch (IOException e) {
//                Log.d(TAG, "burn error : " + e.toString());
//                e.printStackTrace();
//            }
//        } else {
//            Log.d(TAG, "burn error file is not exist ");
//        }
//    }

    /**
     * 获取存储升级包路径。
     *
     * @return String 升级包路径
     */
//    static String getStorePath() {
//        String path = Config.UpdatePath();
//        Log.d(TAG, path);
//        String fileName = Config.UpdateFileName();
//        Log.d(TAG, fileName);
//
//        File file = createFile(String.format("%s/%s", path, fileName));
//        if (file != null) {
//            return file.getPath();
//        }
//
//        return String.format("%s/%s", Storage.Context().getFilesDir().getPath(), fileName);
//    }

    /**
     * 创建目录 当目录不存在的时候创建文件，否则返回false
     */
    private static boolean createDir(String path) {
        File file = new File(path);
        if (!file.exists()) {
            if (!file.mkdirs()) {
                Log.d(TAG, "创建失败，请检查路径和是否配置文件权限！");
                return false;
            }
            return true;
        }
        return false;
    }

    /**
     * 创建文件 当文件不存在的时候就创建一个文件，否则直接返回文件
     */
    private static File createFile(String path) {
        File file = new File(path);
        if (!file.getParentFile().exists()) {
            Log.d(TAG, "目标文件所在路径不存在，准备创建……");
            if (!createDir(file.getParent())) {
                Log.d(TAG, "创建目录文件所在的目录失败！文件路径【" + path + "】");
            }
        }
        // 创建目标文件
        try {
            if (!file.exists()) {
                if (file.createNewFile()) {
                    Log.d(TAG, "创建文件成功:" + file.getAbsolutePath());
                }
                return file;
            } else {
                return file;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    static boolean copyFileByPath(String src, String dst) throws IOException {
        File srcFile = new File(src);
        if (srcFile.exists()) {
            File dstFile = new File(dst);
            return copyFile(srcFile, dstFile);
        }
        return false;
    }

    static boolean copyFile(File src, File dst) throws IOException {
        if (src.getAbsolutePath().equals(dst.getAbsolutePath())) {
            return true;
        } else {
            InputStream is = new FileInputStream(src);
            OutputStream os = new FileOutputStream(dst);
            byte[] buff = new byte[1024];
            int len;
            while ((len = is.read(buff)) > 0) {
                os.write(buff, 0, len);
            }
            is.close();
            os.close();
        }
        return true;
    }

    /**
     * 判断消息栏中是否有本应用的消息
     *
     * @param context
     * @return boolean
     */
    static boolean isNotificationVisible(Context context) {

        if (!NotificationEnabled) {
            return false;
        }

        NotificationManager nm = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

        StatusBarNotification[] sns = nm.getActiveNotifications();

        for (StatusBarNotification sn : sns) {
            Log.d(TAG, sn.getPackageName() + sn.getTag() + sn.getId());
            if (Objects.equals(sn.getPackageName(), context.getPackageName())
                    && Objects.equals(sn.getTag(), TAG_NOTIFICATION)
                    && Objects.equals(sn.getId(), ID_NOTIFICATION)) {
                Log.d(TAG, "isNotificationVisible true");
                return true;
            }
        }
        Log.d(TAG, "isNotificationVisible false");
        return false;
    }

    static void checkUpdate(Context context) {
        switch (Utils.networkType(context)) {
            case WIFI:
            case MOBILE: {
                Intent systemUpdateIntent = new Intent();
                systemUpdateIntent.setAction("syberos.systemupdate.action.CHECK");
                systemUpdateIntent.setPackage("com.syberos.systemupdate");
                context.startService(systemUpdateIntent);
                break;
            }
            default:
                Log.d(TAG, "DISCONNECTED");
                break;
        }
    }

//    static void sendNotification(final Context context, final int textId) {
//        if (!NotificationEnabled) {
//            return;
//        }
//        Log.d(TAG, "sendNotification: " + context.getText(textId));
//        ComponentName componentName;
//        if (Storage.getStatus() < DOWNLOADING) {
//            componentName = new ComponentName(
//                    "com.syberos.systemupdate",
//                    "com.syberos.systemupdate.SystemUpdateActivity");
//        } else {
//            componentName = new ComponentName(
//                    "com.syberos.systemupdate",
//                    "com.syberos.systemupdate.DownloadingActivity");
//        }
//        Intent systemUpdateIntent = new Intent();
//        systemUpdateIntent.setComponent(componentName);
//
//        PendingIntent contentIntent = PendingIntent.getActivity(context,
//                ID_NOTIFICATION, systemUpdateIntent, 0);
//
//        RemoteViews remoteView = new RemoteViews(context.getPackageName(),
//                R.layout.update_notification);
//
//        remoteView.setTextViewText(R.id.notification_text, context.getText(textId));
//
//        Notification notification = new Notification.Builder(context)
//                .setContentTitle(context.getText(R.string.app_name))
//                .setContentText(context.getText(textId))
//                .setTicker(context.getText(textId))
//                .setSmallIcon(R.drawable.systemupdate)
//                .setAutoCancel(false)
//                .setOngoing(true)
//                .setContentIntent(contentIntent)
//                .setPriority(Notification.PRIORITY_MAX)
//                .build();
//
//        notification.contentView = remoteView;
//
//        NotificationManager nm = (NotificationManager)
//                context.getSystemService(Context.NOTIFICATION_SERVICE);
//
//        nm.notify(TAG_NOTIFICATION, ID_NOTIFICATION, notification);
//    }

    static void cancelNotification(final Context context) {
//        if (!Utils.NotificationEnabled) {
//            return;
//        }
        NotificationManager nm = (NotificationManager)
                context.getSystemService(Context.NOTIFICATION_SERVICE);
        nm.cancel(TAG_NOTIFICATION, ID_NOTIFICATION);
    }

   // static void sendProgressNotification(final Context context, int progress) {
   //     if (!NotificationEnabled) {
   //         sendProgressNotificationNoCallActivity(context, progress);
   //         return;
   //     }
   //     ComponentName componentName = new ComponentName("com.syberos.systemupdate",
   //             "com.syberos.systemupdate.DownloadingActivity");
   //     Intent i = new Intent();
   //     i.setComponent(componentName);
   //     PendingIntent mPendingIntent = PendingIntent.getActivity(
   //             context, ID_NOTIFICATION, i, 0);

   //     NotificationManager mNotificationManager =
   //             (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

   //     Notification.Builder builder = new Notification.Builder(
   //             context).setAutoCancel(false)
   //             .setContentTitle(context.getText(R.string.app_name))
   //             .setWhen(System.currentTimeMillis())
   //             .setSmallIcon(android.R.drawable.stat_sys_download)
   //             .setProgress(100, progress, false)
   //             .setContentIntent(mPendingIntent)
   //             .setOngoing(true);
   //     if (context.getResources().getConfiguration().fontScale <= 1) {
   //         String notificationText = String.format("%s...", context.getText(R.string.downloading));
   //         builder.setContentText(notificationText);
   //         builder.setTicker(notificationText);
   //     }
   //     Notification notification = builder.build();

   //     mNotificationManager.notify(TAG_NOTIFICATION, ID_NOTIFICATION, notification);
   // }

   // private static void sendProgressNotificationNoCallActivity(final Context context, int progress) {
   //     NotificationManager mNotificationManager =
   //             (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

   //     Notification.Builder builder = new Notification.Builder(
   //             context).setAutoCancel(false)
   //             .setContentTitle(context.getText(R.string.app_name))
   //             .setWhen(System.currentTimeMillis())
   //             .setSmallIcon(android.R.drawable.stat_sys_download)
   //             .setProgress(100, progress, false)
   //             .setOngoing(true);
   //     if (context.getResources().getConfiguration().fontScale <= 1) {
   //         String notificationText = String.format("%s...", context.getText(R.string.downloading));
   //         builder.setContentText(notificationText);
   //         builder.setTicker(notificationText);
   //     }
   //     Notification notification = builder.build();

   //     mNotificationManager.notify(TAG_NOTIFICATION, ID_NOTIFICATION, notification);
   // }

    static void acquireWakeLock(Context context) {
        Log.d(TAG, "acquireWakeLock");
        if (null == wakeLock) {
            PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
            wakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
            if (null != wakeLock) {
                wakeLock.acquire();
            }
        }
    }

    static void releaseWakeLock() {
        Log.d(TAG, "releaseWakeLock");
        if (wakeLock != null) {
            wakeLock.release();
            wakeLock = null;
        }
    }

    /**
     * Trust every server - dont check for any certificate
     */
    static void trustAllHosts() {
        // Create a trust manager that does not validate certificate chains
        TrustManager[] trustAllCerts = new TrustManager[]{new X509TrustManager() {
            public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                return new java.security.cert.X509Certificate[]{};
            }

            public void checkClientTrusted(X509Certificate[] chain,
                                           String authType) throws CertificateException {
            }

            public void checkServerTrusted(X509Certificate[] chain,
                                           String authType) throws CertificateException {
            }
        }};

        // Install the all-trusting trust manager
        try {
            SSLContext sc = SSLContext.getInstance("TLS");
            sc.init(null, trustAllCerts, new java.security.SecureRandom());
            HttpsURLConnection
                    .setDefaultSSLSocketFactory(sc.getSocketFactory());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    final static HostnameVerifier DO_NOT_HOSTNAME_VERIFY = new HostnameVerifier() {
        @Override
        public boolean verify(String hostname, SSLSession session) {
            return true;
        }
    };

   // static void deleteUpdateFile(){
   //     Log.d(TAG, "deleteUpdateFile");
   //     (new DeleteFileTask()).execute(getStorePath());
   // }

    //////////////////////////////////internal/////////////////////////////////////////
    private Utils() {
    }

    private static class SingletonHolder {
        static final Utils INSTANCE = new Utils();
    }

    private Object readResolve() {
        return getInstance();
    }

    static Utils getInstance() {
        return SingletonHolder.INSTANCE;
    }
    //////////////////////////////////internal/////////////////////////////////////////
}
