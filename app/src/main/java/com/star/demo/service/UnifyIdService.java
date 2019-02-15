package com.star.demo.service;

import java.io.File;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.SocketTimeoutException;
import java.net.URL;
import com.syber.internal.UnifyIdManager;
import com.syber.internal.widget.UnifyIdApp;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Binder;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLException;
import android.content.Context;
import android.content.Intent;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;


public class UnifyIdService extends Service {

    private static final String TAG = "UnifyIdService";

    public Callback callback;
    private DownloadTask mDownloadTask;
    private UnifyIdManager mUnifyIdManager;
    private static final int BUFFER_SIZE = 1024 * 256;
    private static final int UPDATE_DURATION = 500;
    private String mUrl = null;
    private static long mDownloadSize = 0; // 记录当前下载量, 在断点续传时需要获取初始化获取文件大小。
    private static int currentProgress = -1;
    public static final String REQUEST_FEAT_SLIDE_ACTION="com.syberos.FEAT_SLIDE";
    public static final String REQUEST_TRAIN_SLIDE_ACTION="com.syberos.TRAIN_SLIDE";
    public static final String REQUEST_UNIFYID_ACTION="com.syberos.UnifyId";


    interface Callback {

        void updateProgress(long progress);

        void endDownload(boolean succ, String filePath);

        void setTwoStateButtonPaused();

        void setTwoStateButtonResumed();
    }

    private UnifyIdBinder binder = new UnifyIdBinder();

    class UnifyIdBinder extends Binder {

        void register(Callback newCallback) {
            callback = newCallback;
        }

        void unregister() {
            if (callback != null) {
                callback = null;
            }
        }

        public void cancel() {
            //OTALog.d(TAG, "cancel");
            if (mDownloadTask != null) {
                mDownloadTask.cancel(true);
            }
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.d(TAG, "onBind: " + binder);
        return binder;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "onCreate threadID: " + Thread.currentThread().getId()
                + " processID: " + Process.myPid() + " this:" + this);
	System.loadLibrary("unify-id");
	mUnifyIdManager = new UnifyIdManager();
   //     OTALog.d(TAG, "upgrade dir is: " + Utils.getStorePath());
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (intent == null) {
            Log.d(TAG, "onStartCommand intent is null");
            return START_STICKY;
        }
        Log.d(TAG, "onStartCommand processID: " + Process.myPid() + " startId: " + startId);

        //mUrl = Config.DownloadUrl();
        //if (mUrl.isEmpty()) {
        //    mUrl = Storage.url();
        //}

        //OTALog.d(TAG, "status: " + getStatus() + " url: " + mUrl + " totalSize: " + totalSize()
        //        + " mDownloadTask: " + mDownloadTask);

        //if (null != mDownloadTask && mDownloadTask.getStatus() == AsyncTask.Status.RUNNING) {
        //    return START_STICKY;
        //}

        //OTALog.d(TAG, "start new DownloadTask Utils.getStorePath():" + Utils.getStorePath());

        currentProgress = -1;
	Log.d(TAG, "onStartCommand"); 
	UnifyIdRefreshRequest request=new UnifyIdRefreshRequest(this,intent,flags,startId); 
	new Thread(request).start();
	return START_STICKY;
	//return START_REDELIVER_INTENT;


     //   mDownloadTask = new DownloadTask();
      //  mDownloadTask.execute();
       // return START_STICKY;
    }

        public class UnifyIdRefreshRequest implements Runnable{

                private Context mContext;
                private Intent mIntent;
                private int mStartID=-1;
                private int mFlags=-1;
                private static final String TAG="UnifyIdRefreshRequest";

                public UnifyIdRefreshRequest(Context context,Intent intent,int flags,int startId)
                {
                        this.mContext=context;
                        this.mIntent=intent;
                        this.mFlags=flags;
                        this.mStartID=startId;
                }


                @Override
                public void run() {
                        // TODO Auto-generated method stub

                        String action=mIntent.getAction();
                        Log.v(TAG,"run--get action:"+action);
                        if(action==null)
                        {
                                Log.v(TAG,"The service is started without a action");
                                return ;
                        }
                        if(action.equals(REQUEST_FEAT_SLIDE_ACTION))
                        {
				String slide_info=mIntent.getStringExtra("com.syberos.UnifyIdService.info");
				String slide_feature=mIntent.getStringExtra("com.syberos.UnifyIdService.slide_feature");
				String slide_sample=mIntent.getStringExtra("com.syberos.UnifyIdService.slide_sample");
				Log.v(TAG,"feat slide info : "+ slide_info + " feature: "+ slide_feature + " sample: "+slide_sample);
                        	try {
				FileWriter fw_gtouch = new FileWriter("/sdcard//SensorUnifyIdData/slide/GTouch-test.list", true);
                        	fw_gtouch.write(slide_sample+"\n");
                        	fw_gtouch.close();
				} catch (Exception e) {
       			            Log.e(TAG, "finally Exception: " + e.toString());
       			        }

				mUnifyIdManager.featSlide(slide_info, slide_feature);
				String slide_model = new String("/sdcard//UnifyID/module/slide/slideModule");
				double score = mUnifyIdManager.testSlide(slide_model, slide_feature);
				
				Log.i(TAG, "doOnTestSlide: "+score);
				Intent intent = new Intent(UnifyIdManager.UNIFYID_SLIDE_ACTION);
				intent.putExtra(UnifyIdManager.UNIFYID_SLIDE_SCORE, score);
				intent.setFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
				mContext.sendBroadcast(intent);

				File fw_acc = new File("/sdcard//SensorUnifyIdData/slide/GAcc-test.list");
				fw_acc.delete();
				//fw_acc.close();
				File fw_grav = new File("/sdcard//SensorUnifyIdData/slide/GGrav-test.list");
				fw_grav.delete();
				//fw_grav.close();
				File fw_gyro = new File("/sdcard//SensorUnifyIdData/slide/GGyro-test.list");
				fw_gyro.delete();
				File fw_magnet = new File("/sdcard//SensorUnifyIdData/slide/GMagnetics-test.list");
				fw_magnet.delete();
				//fw_gyro.close();
				File fw_touch = new File("/sdcard//SensorUnifyIdData/slide/GTouch-test.list");
				fw_touch.delete();
			    
                               // mLocationClient.start();
                        }
                        if(action.equals(REQUEST_TRAIN_SLIDE_ACTION))
                        {
				String slide_info = new String("/sdcard//SensorUnifyIdData/slide/slide-sample.info");
				String train_features = new String("/sdcard//UnifyID/feature/slide/train/");
				String slide_model = new String("/sdcard//UnifyID/module/slide/slideModule");
				mUnifyIdManager.featSlide(slide_info, train_features);
				mUnifyIdManager.trainSVM(train_features, slide_model);
				int state = 1;
				Intent intent = new Intent(UnifyIdManager.UNIFYID_SLIDE_STATE);
				intent.putExtra(UnifyIdManager.UNIFYID_SLIDE_STATE_MODE, state);
				intent.setFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
				mContext.sendBroadcast(intent);
                            UnifyIdApp.setSlideMode(1);

                               // mLocationClient.start();
                        }

                        if(action.equals(REQUEST_UNIFYID_ACTION))
                        {
                               // mLocationClient.start();
                        }
                }

        }


    @Override
    public void onDestroy() {
        Log.d(TAG, "onDestroy processID: " + Process.myPid());
        super.onDestroy();
    }

    private class DownloadTask extends AsyncTask<Void, Long, Boolean> {

        public void onPreExecute() {
            //OTALog.d(TAG, "onPreExecute current thread id ==" + Thread.currentThread().getId());
          //  Utils.sendNotification(getApplicationContext(), R.string.downloading);
          //  Utils.acquireWakeLock(getApplicationContext());
            //String v = Utils.version(); // for fixed bug 960: because called Looper.prepare() in thread
            //OTALog.d(TAG, "current version: " + v);
        }

        /**
         * @param v
         * @return
         */
        public Boolean doInBackground(Void... v) {
            //OTALog.d(TAG, new Exception().getStackTrace()[0].getMethodName()
            //        + " thread id: " + Thread.currentThread().getId());
            //setStatus(Utils.DOWNLOADING);
            publishProgress(0L);
           // Storage.setDownloadFileNotFound(false);
            return getDelta();
        }

        private void doDownloadSuccess() {
           // setStatus(Utils.DOWNLOADED);
           // Utils.sendNotification(getApplicationContext(), R.string.verifiedTip);
            Intent intent = new Intent("syberos.systemupdate.action.DOWNLOADED");
            sendBroadcast(intent);
            //Storage.setAllowedMobile(false);
          //  if (!Utils.NotificationEnabled) {
          //      Utils.cancelNotification(getApplicationContext());
          //  }
        }

        public void onPostExecute(Boolean success) {
            //OTALog.d(TAG, "onPostExecute: " + success);

            if (success) {
                doDownloadSuccess();
            } else {
        //        Utils.sendNotification(getApplicationContext(), R.string.downloadFailedTip);
        //        if (!Utils.NotificationEnabled) {
        //            Utils.cancelNotification(getApplicationContext());
        //        }
            }

            if (callback != null) {
                //OTALog.d(TAG, "send end download!");
              //  callback.endDownload(success, Utils.getStorePath());
            }
          //  Utils.releaseWakeLock();
        }

        @Override
        protected void onCancelled() {
            //OTALog.d(TAG, "onCancelled processID: " + Process.myPid() + " this: " + this);

            if (mDownloadSize == totalSize()) {
                if (callback != null) {
                //    callback.endDownload(true, Utils.getStorePath());
                }
                doDownloadSuccess();
            } else {
                if (callback != null) {
              //      callback.endDownload(false, Utils.getStorePath());
                }
          //      if (!Utils.NotificationEnabled) {
          //          Utils.cancelNotification(getApplicationContext());
          //      }
            }
        }

        @Override
        public void onProgressUpdate(Long... values) {
            int count = (int) (mDownloadSize * 100 / totalSize());
            if (count != currentProgress) {
                currentProgress = count;
                //OTALog.d(TAG, "currentProgress: " + currentProgress);
            }
         //   Utils.sendProgressNotification(getApplicationContext(), currentProgress);
            if (callback != null) {
                callback.updateProgress(mDownloadSize);
            }
        }

        private boolean getDelta() {
       //     Utils.trustAllHosts();

       //     URL url;
       //     File updateFile = new File(Utils.getStorePath());
       //     RandomAccessFile raf = null;
       //     InputStream in = null;
       //     HttpURLConnection connection = null;
       //     byte[] buffer = new byte[BUFFER_SIZE];
       //     int timeoutTimes = 0;

       //     while (timeoutTimes < Utils.RetryTimes) {
       //         try {
       //             final int status = Storage.getStatus();

       //             mDownloadSize = updateFile.length();
       //             if (mDownloadSize > 0 && Storage.size() > mDownloadSize
       //                     && (status == Utils.DOWNLOAD_PAUSE || status == Utils.DOWNLOADING)) {
       //                 OTALog.d(TAG, "update file has downloaded, current size: " + mDownloadSize);
       //             } else {
       //                 if (updateFile.exists() && updateFile.delete()) {
       //                     String storePath = Utils.getStorePath();
       //                     updateFile = new File(storePath);
       //                     OTALog.d(TAG, "restart download file.");
       //                 }
       //                 mDownloadSize = 0;
       //             }

       //             publishProgress(mDownloadSize);

       //             final long endSize = totalSize();
       //             url = new URL(mUrl);
       //             connection = (HttpURLConnection) url.openConnection();   //建立网络链接
       //             ((HttpsURLConnection) connection).setHostnameVerifier(Utils.DO_NOT_HOSTNAME_VERIFY);
       //             connection.setRequestProperty("Range", "bytes=" + mDownloadSize + "-" + endSize);
       //             OTALog.d(TAG, "Range bytes= " + mDownloadSize + " - " + endSize);
       //             connection.setConnectTimeout(Utils.TimeoutDuration);
       //             connection.setReadTimeout(Utils.TimeoutDuration);        //设置超时时间

       //             raf = new RandomAccessFile(updateFile, "rwd");
       //             raf.seek(mDownloadSize);

       //             int count;
       //             in = connection.getInputStream();
       //             long time = System.currentTimeMillis();

       //             while ((count = in.read(buffer)) != -1) {
       //                 raf.write(buffer, 0, count);
       //                 mDownloadSize += count;

       //                 if (System.currentTimeMillis() - time > UPDATE_DURATION) {
       //                     time = System.currentTimeMillis();
       //                     publishProgress(mDownloadSize);
       //                 }

       //                 if (isCancelled()) {
       //                     return endSize == mDownloadSize;
       //                 }
       //             }

       //             return true;

       //         } catch (SSLException e) {
       //             OTALog.e(TAG, "SSLException: " + e.toString());
       //             Storage.saveDownloadErrorInfo(TAG + " SSLException: " + e.toString());
       //             if (e.toString().contains("Connection reset by peer")) {
       //                 Storage.setDownloadContinue(true);
       //                 Storage.saveDownloadErrorInfo(TAG + " Connection reset by peer");
       //             } else {
       //                 return false;
       //             }
       //         } catch (SocketTimeoutException | ConnectException e) {
       //             OTALog.e(TAG, "SocketTimeoutException: " + e.toString());
       //             if (timeoutTimes < Utils.RetryTimes) {
       //                 Storage.setDownloadContinue(true);
       //                 ++timeoutTimes;
       //                 OTALog.d(TAG, "socketTimeoutTimes: " + timeoutTimes);
       //             } else {
       //                 Storage.saveDownloadErrorInfo(TAG + " SocketTimeoutException: "
       //                         + e.toString());
       //                 return false;
       //             }
       //         } catch (Exception e) {
       //             OTALog.e(TAG, "Exception: " + e.toString());
       //             Storage.saveDownloadErrorInfo(TAG + " Exception: " + e.toString());

       //             if (e.toString().contains("java.io.FileNotFoundException")) {
       //                 Storage.setDownloadFileNotFound(true);
       //             }

       //             return false;
       //         } finally {
       //             OTALog.d(TAG, "release source finally");
       //             try {
       //                 if (raf != null) {
       //                     raf.close();
       //                 }
       //                 if (in != null) {
       //                     in.close();
       //                 }
       //                 if (connection != null) {
       //                     connection.disconnect();
       //                 }
       //             } catch (Exception e) {
       //                 OTALog.e(TAG, "finally Exception: " + e.toString());
       //                 Storage.saveDownloadErrorInfo(TAG + " finally Exception: " + e.toString());
       //             }
       //         }
       //     }
            return false;
        }

    }

    private int getStatus() {
        //return Storage.getStatus();
	return 0;
    }

    private void setStatus(int status) {
    //    Storage.setStatus(status);

    //    if (status == Utils.DOWNLOADING) {
    //        OTALog.d(TAG, "setStatus DOWNLOAD_SERVICE_RUNNING.");
    //        Utils.ContinueDownloading = true;
    //    } else if (status == Utils.DOWNLOADED) {
    //        OTALog.d(TAG, "setStatus Close_this_Service.");
    //        stopSelf();
    //    }
    }

    private long totalSize() {
    //    return Storage.size();
    return 0;
    }

}
