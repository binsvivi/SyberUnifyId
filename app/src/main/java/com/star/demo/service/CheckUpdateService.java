package com.star.demo.service;

import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Build;
import android.os.IBinder;
import java.util.Timer;
import java.util.TimerTask;

import android.provider.Settings;
import android.util.Log;
import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.net.HttpURLConnection;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLException;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;

import com.syber.internal.UnifyIdManager;
import com.syber.internal.widget.UnifyIdApp;

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
public class CheckUpdateService extends Service {
    private CallBack callBack;
    //private CheckUpdateTask checkUpdateTask;
    private Timer timer;
    private static final String PREFERENCES_NAME = "CONFIGURATION_UNIFYID";
    private static SharedPreferences preferences;

    private static final String TAG = "CheckUpdateService";
    public static final String REQUEST_UPDATE_DATA_ACTION="com.syberos.UPDATE_DATA";
    private static boolean startFromUI = false;
    //private TelephonyManager mTManager;
    private WifiManager mWifiManager;
    private String mDeviceId;
    private String mUrl;
    private int mHistoryCount = -1;
    private int mStatus = 0;
    private int mSlideMode = 0;
    private BroadcastReceiver mReceiver;
    private boolean is_upload = false;
    private CheckUpdateBinder binder = new CheckUpdateBinder();

    interface CallBack {
        void startCheck();

        void endCheck(Integer i);
    }

    class CheckUpdateBinder extends Binder {
        CheckUpdateService getService() {
            return CheckUpdateService.this;
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

    private class UnifyIdServiceBroadcastReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.d(TAG, "onReceive " + action);

            if (action.equals(UnifyIdManager.UNIFYID_SLIDE_STATE)) {
                int state = (int)intent.getIntExtra(UnifyIdManager.UNIFYID_SLIDE_STATE_MODE, -1);
                if (state == 1) {
                    int i = UnifyIdApp.SlideCount();
                    Log.d(TAG, "slide count = "+i+"  Slide state = "+state);
                    //if (i == 10)
                 //       startUpload();
                }
                //    mSlideModText.setText("Learning");
;
                mSlideMode = state;
            }

        }
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "onCreate");
        preferences = getApplicationContext().getSharedPreferences(PREFERENCES_NAME, Context.MODE_PRIVATE);
        IntentFilter filter = new IntentFilter();
        filter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(networkStateReceiver, filter);
        mWifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        mHistoryCount = UnifyIdApp.SlideCount();
        mHistoryCount /= UnifyIdApp.SlideSample();

        mReceiver = new UnifyIdServiceBroadcastReceiver();
        IntentFilter intentFilter = new IntentFilter();

        intentFilter.addAction(UnifyIdManager.UNIFYID_GESTURE_ACTION);
        intentFilter.addAction(UnifyIdManager.UNIFYID_SLIDE_ACTION);
        intentFilter.addAction(UnifyIdManager.UNIFYID_SLIDE_COUNT_ACTION);
        intentFilter.addAction(UnifyIdManager.UNIFYID_UNLOCK_ACTION);
        intentFilter.addAction(UnifyIdManager.UNIFYID_TOTAL_ACTION);

        intentFilter.addAction(UnifyIdManager.UNIFYID_GESTURE_STATE);
        intentFilter.addAction(UnifyIdManager.UNIFYID_SLIDE_STATE);
        intentFilter.addAction(UnifyIdManager.UNIFYID_UNLOCK_STATE);
        intentFilter.addAction("android.os.Upload_state");

      //  {
            registerReceiver(mReceiver, intentFilter);

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
        unregisterReceiver(networkStateReceiver);
        unregisterReceiver(mReceiver);
        Intent intent = new Intent("com.syberos.checking_update.destroy");
        sendBroadcast(intent);
    }

    private void startUpload() {
        try {
            int slide_mode = UnifyIdApp.SlideMode();
            String timestamp = Long.toString(System.currentTimeMillis());
        //String deviceId = Utils.getUDID(getApplicationContext());
        //WifiInfo wifiInfo = mWifiManager.getConnectionInfo();
        //String mac = wifiInfo.getMacAddress();
        String androidID = Settings.Secure.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
        String deviceid = androidID + Build.SERIAL;

        String id;
        // int rid = Settings.System.getInt(getApplicationContext().getContentResolver(),"unifyid_random_key", -1);

        // id = String.valueOf(rid);
        id = UnifyIdApp.retrieveKey();
        String BOUNDARY = id;
        String NewLine = "\r\n";
        Log.i(TAG, "deviceId: "+ id + " idInfo: "+ id);
        //mUrl = Config.UploadUrl();
        mUrl = "http://124.192.38.9:6060/upFiles";
        HttpURLConnection connection = null;
        String cm = "tar -zcf /sdcard/SensorUnifyIdData/";
        String mode;
        if (slide_mode == 0)
            mode = "train";
        else
            mode = "test";
        cm += id + "-" + timestamp+"-slide.tar";
        cm += " -C /sdcard//SensorUnifyIdData slide";
        Log.d(TAG, "The cmd is: "+ cm);
        int status = -1;
        try {
            Process p = Runtime.getRuntime().exec(cm);
            status = p.waitFor();
        } catch (InterruptedException e) {
            Log.e(TAG, "Ping test Failed: InterruptedException");
        }
        try {
            Thread.sleep(1000);
        } catch (Exception e) {
            Log.e(TAG, " exception " + e.getMessage());
        }
        URL url = new URL(mUrl);
        Log.d(TAG, "check URL mUrl=" + mUrl + "deviceID= " + id + " status: " + status);
        String up = "/sdcard/SensorUnifyIdData/"+id + "-" + timestamp+"-slide.tar";
        File f = new File(up);
        connection = (HttpURLConnection) url.openConnection();   //建立网络链接
        connection.setRequestMethod("POST");
        connection.setRequestProperty("Connection", "Keep-Alive");
        connection.setRequestProperty("Content-Type", "multipart/form-data; charset=utf-8; boundary="+BOUNDARY);
        //connection.getOutputStream().write();
        connection.setConnectTimeout(Utils.TimeoutDuration);
        connection.setReadTimeout(Utils.TimeoutDuration);
        connection.setDoOutput(true);
        FileInputStream file = new FileInputStream(f);
        byte[] buff = new byte[1024];
        DataInputStream bis = new DataInputStream(file);
        OutputStream os = connection.getOutputStream();
        DataOutputStream bos = new DataOutputStream(os);

        bos.write(("--"+BOUNDARY).getBytes());
        bos.write(NewLine.getBytes());
        String content = "Content-Disposition: form-data; name=\"upfile\"; filename=\""+id+"-"+timestamp+"-slide.tar"+"\"";
        bos.write(content.getBytes());
        bos.write(NewLine.getBytes());
        bos.write(NewLine.getBytes());

        int count = 0;
        while ((count=bis.read(buff)) != -1)
            bos.write(buff,0,count);
        bos.write(NewLine.getBytes());
        bos.write(("--"+BOUNDARY+"--").getBytes());
        bos.write(NewLine.getBytes());
        bos.flush();
        //os.close();
        int result = connection.getResponseCode();
        if (result == 200) {
            InputStream is = connection.getInputStream();
            //String result = StringStreamUtil.inputStreamToString(is);
            cm = "rm /sdcard/SensorUnifyIdData/";
            cm += id + "-" + timestamp+ "-slide.tar";
            Log.d(TAG, "The rm cmd is: "+ cm);
            status = -1;
            // try {
            //     Process p = Runtime.getRuntime().exec(cm);
            //     status = p.waitFor();
            // } catch (InterruptedException e) {
            //      Log.e(TAG, "Rm test Failed: InterruptedException");
            //  }


        }
        Intent intent = new Intent("android.os.Upload_state");
        intent.putExtra("network_state", result);
        UnifyIdApp.setNetworkStatus(result);
        sendBroadcast(intent);
        Log.d(TAG, "result is " + result);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public void createCheckUpdateTask(boolean startFromUI) {
        Log.d(TAG, "createCheckUpdateTask");
        CheckUpdateService.startFromUI = startFromUI;
	timer = new Timer();
	TimerTask task = new TimerTask() {
	   @Override
	   public void run() {
	       try {
	           /*
                   //holder.post(url,"",);
		   FileReader fr = new FileReader("/sdcard//SensorUnifyIdData/slide/slideSample");
		   int len = -1;
		   char[] buffer = new char[1024];
		   //fr.read(buffer);
		   BufferedReader bufferedReader = new BufferedReader(fr);
		   String line = "";
		   line = bufferedReader.readLine(); 
                   Log.d(TAG, "Time check!! "+ line);
		   int i = Integer.parseInt(line);
		   */
           int i = UnifyIdApp.SlideCount();
               Log.d(TAG, "Time check!! slide count: "+ i + "history count="+mHistoryCount);

               i /= UnifyIdApp.SlideSample();
           int slide_mode = UnifyIdApp.SlideMode();
		   if (i != mHistoryCount) {
		       if (mHistoryCount != 0) {
                   mHistoryCount = i;
                   return;
               }
               mHistoryCount = i;

               if (i == 0)
		          return;
		      String timestamp = Long.toString(System.currentTimeMillis());
		      //String deviceId = Utils.getUDID(getApplicationContext());
		      //WifiInfo wifiInfo = mWifiManager.getConnectionInfo();
               //String mac = wifiInfo.getMacAddress();
               String androidID = Settings.Secure.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
               String deviceid = androidID + Build.SERIAL;

               String id;
              // int rid = Settings.System.getInt(getApplicationContext().getContentResolver(),"unifyid_random_key", -1);

              // id = String.valueOf(rid);
                id = UnifyIdApp.retrieveKey();
		      String BOUNDARY = id;
		      String NewLine = "\r\n";
               Log.i(TAG, "deviceId: "+ id + " idInfo: "+ id);
               //mUrl = Config.UploadUrl();
               String base_url;
               base_url = UnifyIdApp.UploadUrl();
               if (base_url.equals("")) {
                   base_url = "124.192.38.9:6060";
               }
               mUrl = "http://"+base_url+"/upFiles";
		      //mUrl = "http://124.192.38.9:6060/upFiles";
		      HttpURLConnection connection = null;
		      String cm = "tar -zcf /sdcard/SensorUnifyIdData/";
		      String mode;
		      if (slide_mode == 0)
		          mode = "train";
		      else
		          mode = "test";
		      cm += id + "-" + timestamp+"-slide.tar";
                      cm += " -C /sdcard//SensorUnifyIdData slide";
                      Log.d(TAG, "The cmd is: "+ cm);
		      int status = -1;
		      try {
		      Process p = Runtime.getRuntime().exec(cm);
		      status = p.waitFor();
		      } catch (InterruptedException e) {
		            Log.e(TAG, "Ping test Failed: InterruptedException");
        		}
                try {
                    Thread.sleep(1000);
                } catch (Exception e) {
		          Log.e(TAG, " exception " + e.getMessage());
                }
                      URL url = new URL(mUrl);
                      Log.d(TAG, "check URL mUrl=" + mUrl + "deviceID= " + id + " status: " + status);
		      String up = "/sdcard/SensorUnifyIdData/"+id + "-" + timestamp+"-slide.tar";
		      File f = new File(up);
		      connection = (HttpURLConnection) url.openConnection();   //建立网络链接
		      connection.setRequestMethod("POST");
		      connection.setRequestProperty("Connection", "Keep-Alive");
		      connection.setRequestProperty("Content-Type", "multipart/form-data; charset=utf-8; boundary="+BOUNDARY);
		      //connection.getOutputStream().write();
                      connection.setConnectTimeout(Utils.TimeoutDuration);
                      connection.setReadTimeout(Utils.TimeoutDuration);
		      connection.setDoOutput(true);
		      FileInputStream file = new FileInputStream(f);
		      byte[] buff = new byte[1024];
		      DataInputStream bis = new DataInputStream(file);
		      OutputStream os = connection.getOutputStream();
		      DataOutputStream bos = new DataOutputStream(os);
		      
		      bos.write(("--"+BOUNDARY).getBytes());
		      bos.write(NewLine.getBytes());
		      String content = "Content-Disposition: form-data; name=\"upfile\"; filename=\""+id+"-"+timestamp+"-slide.tar"+"\"";
		      bos.write(content.getBytes());
		      bos.write(NewLine.getBytes());
		      bos.write(NewLine.getBytes());

		      int count = 0;
		      while ((count=bis.read(buff)) != -1)
		          bos.write(buff,0,count);
		      bos.write(NewLine.getBytes());
		      bos.write(("--"+BOUNDARY+"--").getBytes());
		      bos.write(NewLine.getBytes());
		      bos.flush();
		      //os.close();
		      int result = connection.getResponseCode(); 
		      if (result == 200) {
			 InputStream is = connection.getInputStream();
		         //String result = StringStreamUtil.inputStreamToString(is);
                  cm = "rm /sdcard/SensorUnifyIdData/";
                  cm += id + "-" + timestamp+ "-slide.tar";
                  Log.d(TAG, "The rm cmd is: "+ cm);
                  status = -1;
                 // try {
                 //     Process p = Runtime.getRuntime().exec(cm);
                 //     status = p.waitFor();
                 // } catch (InterruptedException e) {
                //      Log.e(TAG, "Rm test Failed: InterruptedException");
                //  }


		      }
               Intent intent = new Intent("android.os.Upload_state");
               intent.putExtra("network_state", result);
                UnifyIdApp.setNetworkStatus(result);
               sendBroadcast(intent);
		      Log.d(TAG, "result is " + result);
//		      HttpClient httpClient = new DefaultHttpClient();
//		      HttpPost postRequest = new HttpPost(mUrl);
//		      Part[] parts = {
//			      new StringPart("device_id", deviceId),
//			      new StringPart("timestamp", timestamp),
//			      new FilePart("file", file)
//		      };
//		      String up = "/sdcard/SensorUnifyIdData/slide/"+timestamp+"-slide.tar";
//		      //MultipartEntity reqEntity = new MultipartEntity(parts, postRequest.getParams());
//		      MultipartEntity reqEntity = new MultipartEntity();
//		      reqEntity.addPart("file", new FileBody(new File(up), "tar/*"));
		       
//		      postRequest.setEntity(reqEntity);
//		      //HttpResponse res = httpClient.execute(postRequest);
//		      httpClient.execute(postRequest);
		      //res.getEntity().getContent().close(); 
		   }
               } catch (IOException e) {
	           e.printStackTrace();
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
	timer.schedule(task, 0, 1000);
    }

}
