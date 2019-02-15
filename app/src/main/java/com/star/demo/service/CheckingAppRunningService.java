package com.star.demo.service;

import android.app.AppOpsManager;
import android.app.Service;
import android.app.usage.UsageStats;
import android.app.usage.UsageStatsManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.os.Binder;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import android.content.ComponentName;
import android.app.Activity;


import com.star.demo.activity.GestureLoginActivity;
import com.syber.internal.widget.UnifyIdApp;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

//import org.apache.http.impl.client.DefaultHttpClient;
//import org.apache.http.impl.*;
//import org.apache.http.*;
//import org.apache.http.impl.client.*;
//import com.android.internal.http.multipart.FilePart;
//import com.android.internal.http.multipart.MultipartEntity;
//import com.android.internal.http.multipart.Part;
//import com.android.internal.http.multipart.StringPart;
//import org.apache.http.entity.mime.*;
//import org.apache.http.client.methods.HttpPost;
//import org.apache.http.client.HttpClient;
//import org.apache.http.entity.mime.*;
//import org.apache.http.entity.mime.content.FileBody;
//import org.apache.http.entity.mime.content.*;
//import org.apache.http.HttpResponse;
//import org.apache.http.client.HttpClient;
//import org.apache.http.client.methods.HttpPost;
//import org.apache.http.impl.client.DefaultHttpClient;
//import org.apache.http.util.EntityUtils;

/**
 * Created by zhaodongshuang on 2016/07/02.
 */
public class CheckingAppRunningService extends Service {
    private CallBack callBack;
    //private ActivityManager mAm;
    private UsageStatsManager mUSm;
    //private CheckUpdateTask checkUpdateTask;
    private Timer timer;
    private static final String TAG = "CheckingAppRunningService";
    public static final String REQUEST_UPDATE_DATA_ACTION="com.syberos.UPDATE_DATA";
    private static boolean startFromUI = false;
    //private TelephonyManager mTManager;
    private String mDeviceId;
    private String mUrl;
    private String mCurrentApp;
    private int mHistoryCount = -1;
    private int mStatus = 0;
    private CheckUpdateBinder binder = new CheckUpdateBinder();

    interface CallBack {
        void startCheck();

        void endCheck(Integer i);
    }

    class CheckUpdateBinder extends Binder {
        CheckingAppRunningService getService() {
            return CheckingAppRunningService.this;
        }

        void register(CallBack cb) {
            callBack = cb;
        }

        void unregister() {
            callBack = null;
        }

        public void cancel() {
          //  if (null != checkUpdateTask) {
          //      checkUpdateTask.cancel(true);
          //  }
        }

        boolean isStartFromUI() {
            return startFromUI;
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.d(TAG, "onBind" + binder);
        return binder;
    }

    private BroadcastReceiver networkStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
                switch (Utils.networkType(context)) {
                    case Utils.DISCONNECTED: {
                        Log.d(TAG, " network disconnected");
                        //connectChangedToDisconnected();
			mStatus = 0;
                        break;
                    }
                    case Utils.WIFI: {
                        Log.d(TAG, " network has changed to wifi");
                       // connectChangedToWifi();
			mStatus = 1;
                        break;
                    }
                    case Utils.MOBILE: {
                        Log.d(TAG, " network has changed to mobile");
			mStatus = 2;
                       // connectChangedToMobile();
                        break;
                    }
                }
            }
        }
    };

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "onCreate");
        IntentFilter filter = new IntentFilter();
        filter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(networkStateReceiver, filter);
        //mAm = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        mUSm = (UsageStatsManager) getSystemService(Context.USAGE_STATS_SERVICE);
	//mTManager = (TelephonyManager)mContext.getSystemService(Context.TELEPHONY_SERVICE);
        //mDeviceId = mTManager.getDeviceId();
    }

    @Override
    public int onStartCommand(Intent intent, int flag, int startId) {
        Log.d(TAG, "onStartCommand");
        if (intent == null) {
            return START_NOT_STICKY;
        }
        Log.d(TAG, "onStartCommand action" + intent.getAction() + ", startId: " + startId);
        createCheckUpdateTask(false);
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "onDestroy");
        super.onDestroy();
        //if ((checkUpdateTask != null) && !checkUpdateTask.isCancelled()) {
        //    checkUpdateTask.cancel(true);
        //}
        if (timer != null)
            timer.cancel();

        Intent intent = new Intent("com.syberos.checking_app.destroy");
        sendBroadcast(intent);
    }



    public void createCheckUpdateTask(boolean startFromUI) {
        Log.d(TAG, "createCheckUpdateTask");
        CheckingAppRunningService.startFromUI = startFromUI;
	timer = new Timer();
	TimerTask task = new TimerTask() {
	   @Override
	   public void run() {
          // List<RunningTaskInfo> runningTasks = mAm.getRunningTasks(1);
          // RunningTaskInfo runningTaskInfo = runningTasks.get(0);

          // ComponentName topActivity = runningTaskInfo.topActivity;

           //String packageName = topActivity.getPackageName();
           //Log.i(TAG, packageName);

           long now = System.currentTimeMillis();
           //获取60秒之内的应用数据
           List<UsageStats> stats = mUSm.queryUsageStats(UsageStatsManager.INTERVAL_BEST, now - 60 * 1000, now);
          // Log.i(TAG, "Running app number in last 60 seconds : " + stats.size());
           String topActivity = "";
           //取得最近运行的一个app，即当前运行的app
           if ((stats != null) && (!stats.isEmpty())) {
               int j = 0;
               for (int i = 0; i < stats.size(); i++) {
                   if (stats.get(i).getLastTimeUsed() > stats.get(j).getLastTimeUsed()) {
                       j = i;
                   }
               }
               topActivity = stats.get(j).getPackageName();
           }
           //Log.i(TAG, "top running app is : "+topActivity + " current app is" + mCurrentApp);
           if (topActivity.equals(mCurrentApp) ||
                   mCurrentApp == null ||
                   mCurrentApp.equals("com.star.loackpattern")) {
               mCurrentApp = topActivity;
           } else {
               if (mCurrentApp == null) {
                   mCurrentApp = topActivity;
                   return;
               }
               mCurrentApp = topActivity;
               if ((mCurrentApp.equals("com.star.loackpattern") != true)
                       &&  (mCurrentApp.equals("com.android.launcher3") != true)
                       && (mCurrentApp.equals("com.android.systemui") != true)) {
                   Log.i(TAG, "start gesturelogin!!!!!");
                   if (UnifyIdApp.SlideMode() == 1) {
                       Intent intent = new Intent(getApplicationContext(), GestureLoginActivity.class);
                       intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                       startActivity(intent);
                   }
               }

           }



	   }
	};	
        //checkUpdateTask = new CheckUpdateTask() {
        //    @Override
        //    protected void onPreExecute() {
        //        super.onPreExecute();
        //        Log.d(TAG, "onPreExecute");
        //        if (callBack != null) {
        //            callBack.startCheck();
        //        }
        //    }

        //    @Override
        //    protected void onPostExecute(Integer i) {
        //        super.onPostExecute(i);
        //        Log.d(TAG, "onPostExecute: " + i);
        //        if (callBack != null) {
        //            callBack.endCheck(i);
        //        }

        //       // if (i == Utils.HAS_UPDATE) {
        //       //     Intent intent = new Intent("syberos.systemupdate.action.HAS_UPDATE");
        //       //     sendBroadcast(intent);

        //       // } else if (i == Utils.NO_UPDATE) {

        //       //  //   Utils.cancelNotification(getApplicationContext());
        //       // }

        //        Log.d(TAG, "onPostExecute Close_this_Service.");
        //        stopSelf();
        //    }
        //};
        //checkUpdateTask.execute(CheckUpdateService.this);
	timer.schedule(task, 0, 100);
    }

}
