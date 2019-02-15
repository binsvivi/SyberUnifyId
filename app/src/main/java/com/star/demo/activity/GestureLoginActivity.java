package com.star.demo.activity;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Gravity;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.star.demo.R;
import com.star.demo.util.cache.ACache;
import com.star.demo.util.constant.Constant;
import com.star.lockpattern.util.LockPatternUtil;
import com.star.lockpattern.widget.LockPatternView;
import com.star.lockpattern.widget.SyberLockPatternView;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;
import com.syber.internal.UnifyIdManager;
import com.syber.internal.widget.UnifyIdApp;
import com.syber.internal.widget.UnifyIdHelper;

/**
 * Created by Sym on 2015/12/24.
 */
public class GestureLoginActivity extends AppCompatActivity {

    private static final String TAG = "LoginGestureActivity";

    @Bind(R.id.lockPatternView)
    SyberLockPatternView lockPatternView;
    @Bind(R.id.messageTv)
    TextView messageTv;
    @Bind(R.id.forgetGestureBtn)
    Button forgetGestureBtn;

    @Bind(R.id.GestureSettingBtn)
    Button GestureSettingBtn;

    private ACache aCache;
    private static final long DELAYTIME = 600l;
    private byte[] gesturePassword;
    private BroadcastReceiver mReceiver;
    private int mSlideState = 0;
    private double mSlideScore = 0.f;
    private Toast mShowingToast;
    private Vibrator mVibrator;
    private Handler handler;
    private Timer timer;
    private Handler mHandler = new Handler(Looper.getMainLooper());
    private void showToast(String text) {
    // mActivity是一个Activity对象，弹Toast一般用Activity类型的Context
    if (mShowingToast == null) {
        mShowingToast = Toast.makeText(GestureLoginActivity.this, text, Toast.LENGTH_LONG);
    }

    mShowingToast.cancel();
    mShowingToast = null;
    mShowingToast = Toast.makeText(GestureLoginActivity.this, text, Toast.LENGTH_LONG);
    mHandler.postDelayed(new Runnable() {
        @Override
        public void run() {
            mShowingToast.show();   // 会发现延迟之后就显示出来了
        }
    }, 200);  // 这个时间是自己拍脑袋写的，不影响体验就好，试过使用post也不行
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gesture_login);
        ButterKnife.bind(this);
        this.init();
        UnifyIdHelper helper = UnifyIdApp.getInstance();
        helper.AttachedToUnifyIdHelper();
        mSlideState = UnifyIdApp.SlideMode();
        mVibrator=(Vibrator)getApplication().getSystemService(Service.VIBRATOR_SERVICE);
        Log.i(TAG, "LoginOnCreate :"+mSlideState);

        if (mSlideState == 0) {
            if (UnifyIdApp.SlideSample()==120) {
                String text = "请保持坐立，单手持机姿态";
                showToast(text);
                mShowingToast.setGravity(Gravity.TOP,0,0);
                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                lockPatternView.setEnabled(false);
                timer = new Timer();

                handler = new Handler(){
                    public void handleMessage(Message msg) {
                        lockPatternView.setEnabled(true);
                        timer.cancel();
                        super.handleMessage(msg);
                    }

                };

                TimerTask task = new TimerTask() {
                    @Override
                    public void run() {
                        Message message = new Message();
                        message.what = 1;
                        handler.sendMessage(message);
                    }
                };

                timer.schedule(task, 1000, 1000);
            }
        }
    }

    @Override
    protected void onDestroy() {
        Log.i(TAG, "LoginOnDestroy :");
        super.onDestroy();
        UnifyIdHelper helper = UnifyIdApp.getInstance();
        helper.DetachedFromUnifyIdHelper();
        //ButterKnife.bind(this);
        //this.init();
        getApplicationContext().unregisterReceiver(mReceiver);
    }

    private void init() {
        aCache = ACache.get(GestureLoginActivity.this);
        //得到当前用户的手势密码
        gesturePassword = aCache.getAsBinary(Constant.GESTURE_PASSWORD);
        lockPatternView.setOnPatternListener(patternListener);
        updateStatus(Status.DEFAULT);
        mReceiver = new UnifyIdServiceBroadcastReceiver();

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(UnifyIdManager.UNIFYID_SLIDE_ACTION);
        intentFilter.addAction(UnifyIdManager.UNIFYIDDEMO_SLIDE_SWITCH_ACTION);
        intentFilter.addAction(UnifyIdManager.UNIFYID_SLIDE_STATE);
        getApplicationContext().registerReceiver(mReceiver, intentFilter);
    }

    private class UnifyIdServiceBroadcastReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.d(TAG,"ON RECEIVE " +action);

            if (action.equals(UnifyIdManager.UNIFYID_SLIDE_ACTION)) {
                double score = (double) intent.getDoubleExtra(UnifyIdManager.UNIFYID_SLIDE_SCORE,1D);
                Log.d(TAG,"score slide: = " + score + " slide state: =" + mSlideState);
                mSlideScore = score;
                if (mSlideScore > 0.f || mSlideState == 0) {
                    loginGestureSuccess();
                    //GestureLoginActivity.this.finish();
                } else {
                    showToast("非本人身份，解锁操作被拒绝 ");
                    //Toast.makeText(GestureLoginActivity.this, "非本人身份，解锁操作被拒绝 ", Toast.LENGTH_SHORT).show();
                }
            }

            if (action.equals(UnifyIdManager.UNIFYID_SLIDE_STATE)) {
                int state = (int) intent.getIntExtra(UnifyIdManager.UNIFYID_SLIDE_STATE_MODE,1);
                mSlideState = state;
            }

        }
    }

    private SyberLockPatternView.OnPatternListener patternListener = new SyberLockPatternView.OnPatternListener() {

        @Override
        public void onPatternStart() {
            lockPatternView.removePostClearPatternRunnable();
        }

        @Override
        public void onPatternComplete(List<SyberLockPatternView.Cell> pattern) {
            if(pattern != null){
                if(LockPatternUtil.checkPattern(pattern, gesturePassword)) {
                    updateStatus(Status.CORRECT);
                } else {
                    updateStatus(Status.ERROR);
                }
            }
        }
    };

    /**
     * 更新状态
     * @param status
     */
    private void updateStatus(Status status) {
        messageTv.setText(status.strId);
        messageTv.setTextColor(getResources().getColor(status.colorId));
        switch (status) {
            case DEFAULT:
                lockPatternView.setPattern(SyberLockPatternView.DisplayMode.DEFAULT);
                break;
            case ERROR:
                lockPatternView.setPattern(SyberLockPatternView.DisplayMode.ERROR);
                lockPatternView.postClearPatternRunnable(DELAYTIME);
                if (mSlideState == 0) {
                    Log.d("Login", "slidecount= "+UnifyIdApp.SlideCount());
                    String text0 = "当前学习进度为" + UnifyIdApp.SlideCount() + "/"+UnifyIdApp.SlideSample();
                    showToast(text0);
                    int count = UnifyIdApp.SlideCount();
                    int sample = UnifyIdApp.SlideSample();
                    if (sample == 120) {
                        switch (count) {
                            case 20: {
                                String text = "请保持坐立，双手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);
                            }
                            break;
                            case 40: {
                                String text = "请保持站立，单手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);
                            }
                            break;
                            case 60: {
                                String text = "请保持站立，双手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);

                            }
                            break;
                            case 80: {
                                String text = "请保持行走，单手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);
                            }
                            break;
                            case 100: {
                                String text = "请保持行走，双手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);

                            }
                            break;
                            default:
                                return;
                        }

                    }
                    //loginGestureSuccess();
                    //this.finish();
                }
                break;
            case CORRECT:
                lockPatternView.setPattern(SyberLockPatternView.DisplayMode.DEFAULT);
                if (mSlideState == 0) {
                    Log.d("Login", "slidecount= "+UnifyIdApp.SlideCount());
                    String text0 = "Success! 当前学习进度为" + UnifyIdApp.SlideCount() + "/"+UnifyIdApp.SlideSample();
                    showToast(text0);
                    int count = UnifyIdApp.SlideCount();
                    int sample = UnifyIdApp.SlideSample();
                    if (sample == 120) {
                        switch (count) {
                            case 20: {
                                String text = "请保持坐立，双手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);

                            }
                                break;
                            case 40: {
                                String text = "请保持站立，单手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);


                            }
                                break;
                            case 60: {
                                String text = "请保持站立，双手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);

                            }
                                break;
                            case 80: {
                                String text = "请保持行走，单手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);
                            }
                                break;
                            case 100: {
                                String text = "请保持行走，双手持机姿态";
                                showToast(text);
                                mShowingToast.setGravity(Gravity.TOP, 0,0);
                                mVibrator.vibrate(new long[]{100,100,100,1000},-1);
                                lockPatternView.setEnabled(false);
                                timer = new Timer();

                                handler = new Handler(){
                                    public void handleMessage(Message msg) {
                                        lockPatternView.setEnabled(true);
                                        timer.cancel();
                                        super.handleMessage(msg);
                                    }

                                };

                                TimerTask task = new TimerTask() {
                                    @Override
                                    public void run() {
                                        Message message = new Message();
                                        message.what = 1;
                                        handler.sendMessage(message);
                                    }
                                };

                                timer.schedule(task, 1000, 1000);
                            }
                                break;
                            default:
                                return;
                        }

                    }
                    //loginGestureSuccess();
                    //this.finish();
                }
                break;
        }
    }

    /**
     * 手势登录成功（去首页）
     */
    private void loginGestureSuccess() {
        showToast("成功验证本人身份");
       // Toast.makeText(GestureLoginActivity.this, "success", Toast.LENGTH_SHORT).show();
    }

    /**
     * 忘记手势密码（去账号登录界面）
     */
    @OnClick(R.id.forgetGestureBtn)
    void forgetGesturePasswrod() {
        {
        String action = UnifyIdManager.UNIFYIDDEMO_SLIDE_SWITCH_ACTION;
        UnifyIdApp.setSlideMode(0);
        UnifyIdHelper helper = UnifyIdApp.getInstance();
        helper.SwitchToLearn(2);
        Intent intent = new Intent(action);
        intent.putExtra(UnifyIdManager.UNIFYID_SWITCH_STATE, 0);
        intent.setFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
        getApplicationContext().sendBroadcast(intent);
        }

        Intent intent = new Intent(GestureLoginActivity.this, CreateGestureActivity.class);
        startActivity(intent);
        this.finish();
    }

    @OnClick(R.id.GestureSettingBtn)
    void GestureStudySetting() {
        Log.d(TAG, "study setting!");
        Intent intent = new Intent(GestureLoginActivity.this, StudySettingActivity.class);
        startActivity(intent);
      //  this.finish();
    }

    private enum Status {
        //默认的状态
        DEFAULT(R.string.gesture_default, R.color.grey_a5a5a5),
        //密码输入错误
        ERROR(R.string.gesture_error, R.color.red_f4333c),
        //密码输入正确
        CORRECT(R.string.gesture_correct, R.color.grey_a5a5a5);

        private Status(int strId, int colorId) {
            this.strId = strId;
            this.colorId = colorId;
        }
        private int strId;
        private int colorId;
    }
}
