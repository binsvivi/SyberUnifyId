package com.star.demo.activity;

import com.star.demo.service.*;
import com.star.demo.R;

import android.Manifest;
import android.app.Activity;
import android.app.ActivityManager;

import android.app.AppOpsManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.PowerManager;
import android.os.RemoteException;
import android.os.SystemClock;
//import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.os.UserHandle;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;

import android.content.ContentResolver;
import android.content.Context;

import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import android.view.*;
//import com.android.internal.widget.PointerLocationView;
//import android.view.InputEventReceiver;
//import com.android.internal.view.BaseIWindow;
import android.graphics.Rect;
import android.graphics.PixelFormat;
import android.view.WindowManager;
import android.view.Window;
//import com.android.internal.*;
//import com.syber.internal.widget.UnifyIdView;
import com.star.lockpattern.widget.RoundProgressBar;
import com.syber.internal.UnifyIdManager;
import com.syber.internal.widget.UnifyIdApp;
import com.syber.internal.widget.UnifyIdHelper;

import android.widget.ArrayAdapter;
//import com.android.server.wm.WindowManagerService;
//import android.view.WindowManagerGlobal;
//import com.android.server.wm.*;
import java.lang.reflect.Method;
import android.view.View.OnClickListener;

import android.provider.Settings;

import static android.content.ContentValues.TAG;
//import static com.star.demo.service.Config.*;

//import android.os.ServiceManager;
public class StudySettingActivity extends Activity implements SensorEventListener, OnClickListener {
    private static final String PREFERENCES_NAME = "CONFIGURATION_UNIFYID";
    private static SharedPreferences preferences;


    private SensorManager sensorManager;
    private SensorData sensorData;
    private List<SensorData> sensorDataList;
    private List<SensorData> GravitySensorDataList;
    private List<SensorData> AccSensorDataList;
    private List<SensorData> GyroSensorDataList;
    private List<TouchData> touchDataList;
    private int num = 5000;
    private int pointNum = 0;
    private int repeat = this.repeatNum;
    private int repeatNum = 1;
    private Timer timer;
    private Timer timerUp;
    private Timer timerAll;
    private int interval = 0;
    private boolean isAppRunning;
    private String FilePath;
    private long sampleTimestamp = 0;
    private boolean screen_on = false;
    //private PointerLocationView mPointerLocationView;
    //private UnifyIdView mUnifyIdView;
    private Button mOpenUnifyId;
    private Button mReturnFromUnifyId;
    private Button mCloseUnifyId;
    private Button mKeyBtn;
    private TextView mGestureModText;
    private TextView mUnifyIdText;
    private TextView mUploadText;
    private TextView mUnlockModText;
    private TextView mSlideModText;
    private TextView mGestureVerText;
    private TextView mUnlockVerText;
    private TextView mSlideVerText;
    private TextView mTotalVerText;
    private EditText mUploadUrl;
    private static final int REQUESTCODE = 100;
    private RoundProgressBar roundProgressBar;
    private int mGestureMode = 0;
    private int mSlideMode = 0;
    private boolean mSlideSpinnerFirstBoot = false;
    private int mUnlockMode = 0;
    private double mGestureScore = 0.f;
    private double mSlideScore = 0.f;
    private double mUnlockScore = 0.f;
    private double mTotalScore = 0.f;
    private double mSlideCount = 0.f;
    private String mRandomKey;
    private Spinner gesturemodelspinner;
    private Spinner unlockmodelspinner;
    private Spinner slidemodelspinner;
    private List<String> gesture_model_list;
    private ArrayAdapter<String> gesture_model_adapter;
    private ArrayAdapter<String> slide_model_adapter;
    private ArrayAdapter<String> unlock_model_adapter;
    private BroadcastReceiver mReceiver;
    private Handler handler;
    private CharSequence temp;
   // IWindowSession mSession;
  //  InputChannel mInputChannel;
    final WindowManager.LayoutParams mLayout = new WindowManager.LayoutParams();
    final Rect mOutsets = new Rect();
    final Rect mStableInsets = new Rect();
    final Rect mContentInsets = new Rect();
  //  UnifyIdInputEventReceiver mInputEventReceiver;
/*
    final BaseIWindow mWindow = new BaseIWindow() {
        @Override
        public void resized(Rect rect, Rect rect1, Rect rect2, Rect rect3, Rect rect4, Rect rect5, boolean b, Configuration configuration) {
            super.resized(rect, rect1, rect2, rect3, rect4, rect5, b, configuration);
        }

        @Override
        public void moved(int i, int i1) {
            super.moved(i, i1);
        }

        @Override
        public void dispatchAppVisibility(boolean b) {
            super.dispatchAppVisibility(b);
        }

        @Override
        public void dispatchWallpaperCommand(String s, int i, int i1, int i2, Bundle bundle, boolean b) {
            super.dispatchWallpaperCommand(s, i, i1, i2, bundle, b);
        }

        @Override
        public void dispatchWallpaperOffsets(float v, float v1, float v2, float v3, boolean b) {
            super.dispatchWallpaperOffsets(v, v1, v2, v3, b);
        }
    };
*/
    private class SensorData
    {
        public long accelerometerTimeStamp;
        public float accelerometerX;
        public float accelerometerY;
        public float accelerometerZ;
        public long gravityTimeStamp;
        public float gravityX;
        public float gravityY;
        public float gravityZ;
        public long gyroscopeTimeStamp;
        public float gyroscopeX;
        public float gyroscopeY;
        public float gyroscopeZ;
        public long pressureTimeStamp;
        public float pressure;

        private SensorData() {}
    }
    private class TouchData
    {
        public long timestamp;
        public float touchX;
        public float touchY;
        public float pressure;

        private TouchData() {}
    }
/*
    private class UnifyIdInputEventReceiver extends InputEventReceiver {
        public UnifyIdInputEventReceiver(InputChannel inputChannel, Looper looper){
            super(inputChannel, looper);
        }

        public void onInputEvent(InputEvent event) {
            boolean handled = false;
            try {
                if (event instanceof MotionEvent && (event.getSource() & InputDevice.SOURCE_CLASS_POINTER) != 0) {
                    final MotionEvent motionEvent = (MotionEvent)event;
                    if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
                        System.out.println("---Input action move-----");

                    } else if (motionEvent.getAction() == MotionEvent.ACTION_MOVE){
                        System.out.println("---Input action move-----");
                    } else if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                        System.out.println("---Input action move-----");
                    }
                    handled = true;
                }
            } finally {
                finishInputEvent(event, handled);
            }
        }
    }
*/
    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("native-lib");
//    }

    @Override
    protected void onResume(){
        System.out.println("onResume");
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        unregisterReceiver(mReceiver);
        timer.cancel();
    }

    private class UnifyIdServiceBroadcastReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
	    String action = intent.getAction();
	    Log.d("UnifyId-UI", "onReceive " + action);
            System.out.println("unifyid receive!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	 if (action.equals(UnifyIdManager.UNIFYID_TOTAL_ACTION)) {
	 	double score = (double)intent.getDoubleExtra(UnifyIdManager.UNIFYID_TOTAL_SCORE,1d);

//		mTotalScore = 0.f;
//		if (mGestureMode == 1)
//			mTotalScore += 0.8 * mGestureScore;
//		if (mUnlockMode == 1)
//			mTotalScore += 0.9 * mUnlockScore;
//		if (mSlideMode == 1)
//			mTotalScore += 0.9 * mSlideScore;

	    	Log.d("UnifyId-UI", "UNIFYID_TOTAL_ACTION UNIFYID_TOTAL_SCORE: " + score);
		if (score >= 0.f)
			mTotalVerText.setText("Pass");
		else
			mTotalVerText.setText("Denied");
	 }
	 if (action.equals(UnifyIdManager.UNIFYID_UNLOCK_ACTION)) {
	 	double score = (double)intent.getDoubleExtra(UnifyIdManager.UNIFYID_UNLOCK_SCORE,1d);
            	System.out.println("score : == " + score);
		if (score >= 0.f)
		mUnlockVerText.setText("Pass");
		else
		mUnlockVerText.setText("Denied");
		mUnlockScore = score;
	 }
	 if (action.equals(UnifyIdManager.UNIFYID_SLIDE_COUNT_ACTION)) {
                int count = (int)intent.getIntExtra(UnifyIdManager.UNIFYID_SLIDE_COUNT,1);
                System.out.println("count : == " + count);
                double progress = (double)count / UnifyIdApp.SlideSample();
                progress = progress * 100;
         System.out.println("progress : == " + progress);
         roundProgressBar.setProgress((int)progress);
                mSlideCount = count;
     }

	 if (action.equals(UnifyIdManager.UNIFYID_SLIDE_ACTION)) {
	 	double score = (double)intent.getDoubleExtra(UnifyIdManager.UNIFYID_SLIDE_SCORE, 1d);
            	System.out.println("score : == " + score);
		if (score >= 0.f)
		mSlideVerText.setText("Pass");
		else
		mSlideVerText.setText("Denied");
		mSlideScore = score;
		
//		mTotalScore = 0.f;
//		if (mGestureMode == 1)
//			mTotalScore += 0.8 * mGestureScore;
//		if (mUnlockMode == 1)
//			mTotalScore += 0.9 * mUnlockScore;
//		if (mSlideMode == 1)
//			mTotalScore += 0.9 * mSlideScore;
//
//		if (mTotalScore >= 0.f)
//			mTotalVerText.setText("Pass");
//		else
//			mTotalVerText.setText("Denied");
	 }
 	 if (action.equals(UnifyIdManager.UNIFYID_GESTURE_ACTION)) {
	 	double score = (double)intent.getDoubleExtra(UnifyIdManager.UNIFYID_GESTURE_SCORE, 1d);
            	System.out.println("score : == " + score);
		if (score >= 0.f)
		    mGestureVerText.setText("Pass");
		else
		    mGestureVerText.setText("Denied");
		mGestureScore = score;
	 }
 	 if (action.equals(UnifyIdManager.UNIFYID_UNLOCK_STATE)) {
	 	int state = (int)intent.getIntExtra(UnifyIdManager.UNIFYID_UNLOCK_STATE_MODE,1);
		if (state == 0)
		mUnlockModText.setText("Learning");
		else
		mUnlockModText.setText("Testing");
		mUnlockMode = state;
	 }
	 if (action.equals(UnifyIdManager.UNIFYID_SLIDE_STATE)) {
	 	int state = (int)intent.getIntExtra(UnifyIdManager.UNIFYID_SLIDE_STATE_MODE, 1);
	 	if (state == 0) {
            mSlideModText.setText("Learning");
            mCloseUnifyId.setText("进入学习");
        }
		else {
            mSlideModText.setText("Testing");
            mCloseUnifyId.setText("进入验证");
        }
		mSlideMode = state;
	 }
 	 if (action.equals(UnifyIdManager.UNIFYID_GESTURE_STATE)) {
	 	int state = (int)intent.getIntExtra(UnifyIdManager.UNIFYID_GESTURE_STATE_MODE, 1);
		if (state == 0)
		mGestureModText.setText("Learning");
		else
		mGestureModText.setText("Testing"); 
		mGestureMode = state;
	}
	if (action.equals("android.os.Upload_state")) {
                int state = (int)intent.getIntExtra("network_state", 0);
                String result = String.valueOf(state);
                    mUploadText.setText(result);
                //else
                //    mGestureModText.setText("Testing");
                //mGestureMode = state;

    }
	} 
    }
/*
    private void enablePointerLocation() {

	    if (mUnifyIdView == null) {
		    mUnifyIdView = new UnifyIdView(getApplicationContext());
		    mUnifyIdView.setPrintCoords(false);
		    WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
				    WindowManager.LayoutParams.MATCH_PARENT,
				    WindowManager.LayoutParams.MATCH_PARENT);
		    lp.type = WindowManager.LayoutParams.TYPE_SECURE_SYSTEM_OVERLAY;
		    lp.flags = WindowManager.LayoutParams.FLAG_FULLSCREEN
			    | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
			    | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
			    | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN;
		    if (ActivityManager.isHighEndGfx()) {
			    lp.flags |= WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED;
			    lp.privateFlags |=
				    WindowManager.LayoutParams.PRIVATE_FLAG_FORCE_HARDWARE_ACCELERATED;
		    }
		    lp.format = PixelFormat.TRANSLUCENT;
		    lp.setTitle("UnifyId");
		    WindowManager wm = (WindowManager)
			    getApplicationContext().getSystemService(Context.WINDOW_SERVICE);
		    lp.inputFeatures |= WindowManager.LayoutParams.INPUT_FEATURE_NO_INPUT_CHANNEL;
		    wm.addView(mUnifyIdView, lp);
		//    mWindowManagerFuncs.registerPointerEventListener(mUnifyIdView);
		    IWindowManager sWindowManagerService = IWindowManager.Stub.asInterface(ServiceManager.getService("window"));
		    try {
		    Class c;
		    c = Class.forName("com.android.server.wm.WindowManagerService");
		    Method m = c.getMethod("registerPointerEventListener", UnifyIdView.class);
		    m.invoke(sWindowManagerService, mUnifyIdView);
		   // ((WindowManagerService)sWindowManagerService).registerPointerEventListener(mUnifyIdView);
		      } catch (Exception e) {
		    	throw new RuntimeException("Failed to invoke registerPointerEventListener", e);
		    }
	    }
    }
*/

    private String generate_random_key() {
        int num = (int) ((Math.random() * 9 + 1) * 100000);
        //Settings.System.putInt(getApplicationContext().getContentResolver(),"unifyid_random_key",num);
        String random_key = String.valueOf(num);
        preferences.edit().putString("KEY_CONFIG_UNIFYID_KEY", random_key);
        return random_key;
    }

    @Override
    public void onClick(View v) {
	if ( v == mOpenUnifyId ) {
            Log.d("Demo", "onClick open");
  //          return;
          //  enablePointerLocation();
	    //Settings.System.putInt(getApplicationContext().getContentResolver(),"unifyid_boot",1);
	    //mUnifyIdText.setText("Open");

        String action = UnifyIdManager.UNIFYIDDEMO_SLIDE_SWITCH_ACTION;
        UnifyIdApp.setSlideMode(0);
        UnifyIdHelper helper = UnifyIdApp.getInstance();
        helper.SwitchToLearn(2);
        Intent intent = new Intent(action);
        intent.putExtra(UnifyIdManager.UNIFYID_SWITCH_STATE, 0);
        intent.setFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
    //    getApplicationContext().sendBroadcast(intent);

        //int count = UnifyIdApp.SlideCount();
        double progress = (double)0 / UnifyIdApp.SlideSample();
        progress = progress * 100;
        System.out.println("progress : == " + progress);
        roundProgressBar.setProgress((int)progress);
        mSlideCount = 0;
        UnifyIdApp.setNetworkStatus(0);

    }


	if ( v ==  mCloseUnifyId) {
            Log.d("Demo", "onClick close");
          //  enablePointerLocation();
	//    Settings.System.putInt(getApplicationContext().getContentResolver(),"unifyid_boot",0);
	    //mUnifyIdText.setText("Close");
	    mGestureModText.setText("");
	    mGestureVerText.setText("");
	    mUnlockModText.setText("");
	    mUnlockVerText.setText("");
	    mSlideModText.setText("");
	    mSlideVerText.setText("");

        //String action = UnifyIdManager.UNIFYIDDEMO_SLIDE_SWITCH_ACTION;
        //UnifyIdApp.setSlideMode(1);
        //Intent intent = new Intent(action);
       // intent.putExtra(UnifyIdManager.UNIFYID_SWITCH_STATE, 1);
       // intent.setFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
       // getApplicationContext().sendBroadcast(intent);
       // if (mSlideMode == 0) {
          //  this.finish();
            Intent intent = new Intent(getApplicationContext(), GestureLoginActivity.class);
            startActivity(intent);


        //}
        }

        if (v == mReturnFromUnifyId) {
            Log.d("Demo", "onReturn close");
	        this.finish();
        }

        if (v == mKeyBtn) {
	        String random_key = UnifyIdApp.generateRandomKey();
	        Log.d("Demo", "random_key: "+random_key);
            mUnifyIdText.setText(random_key);
            mRandomKey = random_key;

        }


    }

        class ModelSpinnerSelectedListener implements OnItemSelectedListener {
                public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
                    if (mSlideSpinnerFirstBoot == false) {
                        mSlideSpinnerFirstBoot = true;
                        return;
                    }
			       String action = new String();
			       if (arg0 == slidemodelspinner) {
				    action = UnifyIdManager.UNIFYIDDEMO_SLIDE_SWITCH_ACTION;
                     //  UnifyIdApp.setSlideMode(position);
                       switch (position) {
                           case 0:
                               UnifyIdApp.setSlideSample(50);
                               break;
                           case 1:
                               UnifyIdApp.setSlideSample(40);
                               break;
                           case 2:
                               UnifyIdApp.setSlideSample(30);
                               break;
                           case 3:
                               UnifyIdApp.setSlideSample(20);
                               break;
                           case 4:
                               UnifyIdApp.setSlideSample(10);
                               break;
                           case 5:
                               UnifyIdApp.setSlideSample(120);
                               break;
                           default:
                               return;

                       }

                   }
			       if (arg0 == gesturemodelspinner) {
       				    //   intent = new Intent(UnifyIdManager.UNIFYIDDEMO_GESTURE_SWITCH_ACTION);
				    action = UnifyIdManager.UNIFYIDDEMO_GESTURE_SWITCH_ACTION;
				}
			       if (arg0 == unlockmodelspinner) {
       				    //   intent = new Intent(UnifyIdManager.UNIFYIDDEMO_UNLOCK_SWITCH_ACTION);
				    action = UnifyIdManager.UNIFYIDDEMO_UNLOCK_SWITCH_ACTION;
				} 
		               if (position == 0) {
                                        Log.d("MainActivity",
                                                        "########################################train_mode --------------------------"+action);
                                } else if (position == 1) {
                                        Log.d("MainActivity",
                                                        "########################################verify_only --------------------------" + action);

                                }

       				// Intent intent = new Intent(action);
       				// intent.putExtra(UnifyIdManager.UNIFYID_SWITCH_STATE, position);
       				// intent.setFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
       				// getApplicationContext().sendBroadcast(intent);
                               // usbmodeTxt.setText(str);

                }
                public void onNothingSelected(AdapterView<?> arg0) {
                }
        }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        preferences = getApplicationContext().getSharedPreferences(PREFERENCES_NAME, Context.MODE_PRIVATE);
        int sample = UnifyIdApp.SlideSample();
        if (sample == 0)
        UnifyIdApp.setSlideSample(50);
//    	if (MainActivity.this.mPointerLocationView == null) {
  //  	MainActivity.this.mPointerLocationView = new PointerLocationView(getApplicationContext());
	final View dialogBackground = View.inflate(getApplicationContext(),R.layout.update_dialog_back, null);
	mOpenUnifyId = (Button) this.findViewById(R.id.btn1);
        mOpenUnifyId.setOnClickListener(this);
        mReturnFromUnifyId = (Button) this.findViewById(R.id.return_btn);
        mReturnFromUnifyId.setOnClickListener(this);
	mCloseUnifyId = (Button) this.findViewById(R.id.btn2);
        mCloseUnifyId.setOnClickListener(this);
        int mode = UnifyIdApp.SlideMode();
        if (mode == 0)
            mCloseUnifyId.setText("进入学习");
        else
            mCloseUnifyId.setText("进入认证");

        mUploadUrl = (EditText) this.findViewById(R.id.uploadUrl);

        TextWatcher textWatcher = new TextWatcher() {
            // 输入文本之前的状态
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            // 输入文本中的状态
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                temp = s; //temp = s   用于记录当前正在输入文本的个数

            }

            // 输入文本之后的状态
            @Override
            public void afterTextChanged(Editable s) {
                // editStart = mUploadUrl.getSelectionStart();
                // editEnd = mUploadUrl.getSelectionEnd();
                UnifyIdApp.setUploadUrl(temp.toString());
                //mUploadUrl.setText("" + temp.length());  //把输入temp中记录的字符个数显示在TextView上
                // if (temp.length() >10) {
                //     s.delete(editStart - 1, editEnd);
                //     int tempSelection = editStart;
                //     main_et.setText(s);
                //     main_et.setSelection(tempSelection);
                // }
            }
        };
        mUploadUrl.addTextChangedListener(textWatcher);
        mUnifyIdText = (TextView) this.findViewById(R.id.value0);
	mUploadText = (TextView) this.findViewById(R.id.uploadvalue);
    mKeyBtn = (Button) this.findViewById(R.id.keybtn);
        mKeyBtn.setOnClickListener(this);

        mGestureModText = (TextView) this.findViewById(R.id.modelvalue1);
	mSlideModText = (TextView) this.findViewById(R.id.modelvalue2);
	mUnlockModText = (TextView) this.findViewById(R.id.modelvalue3);

	mGestureVerText = (TextView) this.findViewById(R.id.verifyvalue1);
	mSlideVerText = (TextView) this.findViewById(R.id.verifyvalue2);
	mUnlockVerText = (TextView) this.findViewById(R.id.verifyvalue3);
	mTotalVerText = (TextView) this.findViewById(R.id.verifyvalue4);

	gesturemodelspinner = (Spinner) findViewById(R.id.gesturemodelspinner);
	slidemodelspinner = (Spinner) findViewById(R.id.slidemodelspinner);
	unlockmodelspinner = (Spinner) findViewById(R.id.unlockmodelspinner);

	roundProgressBar = (RoundProgressBar) findViewById(R.id.roundProgressBar);
	roundProgressBar.setVisibility(View.VISIBLE);
	int count = UnifyIdApp.SlideCount();
        double progress = (double)count / UnifyIdApp.SlideSample();
        progress = progress * 100;
        System.out.println("progress : == " + progress);
        roundProgressBar.setProgress((int)progress);
        mSlideCount = count;


        gesture_model_list =new ArrayList<String>();
        gesture_model_list.add("50");
        gesture_model_list.add("40");
        gesture_model_list.add("30");
        gesture_model_list.add("20");
        gesture_model_list.add("10");
        gesture_model_list.add("120");

        gesture_model_adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, gesture_model_list);
        gesture_model_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        gesturemodelspinner.setEnabled(false);
        gesturemodelspinner.setAdapter(gesture_model_adapter);
        gesturemodelspinner.setOnItemSelectedListener(new ModelSpinnerSelectedListener());

//        gesture_model_list =new ArrayList<String>();
//        gesture_model_list.add("train");
//        gesture_model_list.add("verify");
        unlock_model_adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, gesture_model_list);
        unlock_model_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        unlockmodelspinner.setEnabled(false);
        unlockmodelspinner.setAdapter(unlock_model_adapter);
        unlockmodelspinner.setOnItemSelectedListener(new ModelSpinnerSelectedListener());

  //      gesture_model_list =new ArrayList<String>();
  //      gesture_model_list.add("train");
  //      gesture_model_list.add("verify");
        slide_model_adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, gesture_model_list);
        slide_model_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        slidemodelspinner.setEnabled(true);
        slidemodelspinner.setAdapter(slide_model_adapter);
        slidemodelspinner.setOnItemSelectedListener(new ModelSpinnerSelectedListener());
        int slide_sample = UnifyIdApp.SlideSample();
        int position = 0;
        switch (slide_sample) {
            case 50:
                position = 0;
                break;
            case 40:
                position = 1;
                break;
            case 30:
                position = 2;
                break;
            case 20:
                position = 3;
                break;
            case 10:
                position = 4;
                break;
            case 120:
                position = 5;
                break;
                default:
                    position = 0;
                    break;
        }
        slidemodelspinner.setSelection(position, true);

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

        {
        registerReceiver(mReceiver, intentFilter);
	Intent intent = new Intent(getApplicationContext(), UnifyIdService.class);
	Log.d("UnifyId-UI", "restart UnifyId Service when it's not running.");
	getApplicationContext().startService(intent);
        }

	{
	Intent intent = new Intent(getApplicationContext(), CheckUpdateService.class);
	Log.d("UnifyId-UI", "restart CheckUpdate Service when it's not running.");
	getApplicationContext().startService(intent);
	}

	{
        Intent intent = new Intent(getApplicationContext(), CheckingAppRunningService.class);
        Log.d("UnifyId-UI", "restart CheckAppRunning Service when it's not running.");
        getApplicationContext().startService(intent);
    }

        //UnifyIdHelper helper = UnifyIdApp.getInstance();
        //helper.AttachedToUnifyIdHelper();
        mSlideMode = UnifyIdApp.SlideMode();
        Log.i(TAG, "StudyOnCreate :"+mSlideMode);
        if (mSlideMode == 0)
            mSlideModText.setText("Learning");
        else
            mSlideModText.setText("Testing");

       // String deviceId = Utils.getUDID(getApplicationContext());
        String androidID = Settings.Secure.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
        String deviceid = androidID + Build.SERIAL;
       // mUnifyIdText.setText(id);

        String random_key;
       // int id = Settings.System.getInt(getApplicationContext().getContentResolver(),"unifyid_random_key", -1);
              random_key = UnifyIdApp.retrieveKey();
        //if (id == -1) {
        //    random_key = generate_random_key();
        //}else
       //     random_key = String.valueOf(id);
        Log.d("demo", "on_create random_key: "+random_key);
        if (random_key.equals("null"))
            random_key = UnifyIdApp.generateRandomKey();
        mUnifyIdText.setText(deviceid+"-"+random_key);
        mRandomKey = random_key;

        timer = new Timer();

        handler = new Handler(){
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case 1:
                int networkstatus = UnifyIdApp.NetworkStatus();
                String status = String.valueOf(networkstatus);
                mUploadText.setText(status);
                break;
            }
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

        timer.schedule(task, 0, 1000);

        //	MainActivity.this.mPointerLocationView.setPrintCoords(false);
 //   	WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
 //   			WindowManager.LayoutParams.MATCH_PARENT,
 //   			WindowManager.LayoutParams.MATCH_PARENT);
 //   	lp.type = WindowManager.LayoutParams.TYPE_SECURE_SYSTEM_OVERLAY;
 //   	lp.alpha = 0.5f;
 //       lp.width = WindowManager.LayoutParams.MATCH_PARENT;
 //       lp.height = WindowManager.LayoutParams.MATCH_PARENT;
 //       lp.flags = WindowManager.LayoutParams.FLAG_FULLSCREEN
 //   		| WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
 //   		| WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
 //   		| WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN;
 //   	if (ActivityManager.isHighEndGfx()) {
 //   		lp.flags |= WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED;
 //   		lp.privateFlags |=
 //   			WindowManager.LayoutParams.PRIVATE_FLAG_FORCE_HARDWARE_ACCELERATED;
 //   	}
 //   	lp.format = PixelFormat.TRANSLUCENT;
 //   	lp.setTitle("PointerLocationTest");
 //   	WindowManager wm = (WindowManager)
 //   		getSystemService(Context.WINDOW_SERVICE);
    //	lp.inputFeatures |= WindowManager.LayoutParams.INPUT_FEATURE_NO_INPUT_CHANNEL;
    	
//	 mPointerLocationView.setOnTouchListener(new View.OnTouchListener() {
//            @Override
//            public boolean onTouch(View v, MotionEvent event) {
//                    Log.d("UnifyId", "view onTouch111111");
//                if (event.getAction() == MotionEvent.ACTION_DOWN && event.getY() < 0) {
//                    Log.d("UnifyId", "view onTouch");
//                }
//                return false;
//            }
//        });

//	wm.addView(mPointerLocationView, lp);
    	//mWindowManagerFuncs.registerPointerEventListener(mPointerLocationView);
   // 	}

//    	mReceiver = new UnifyIdServiceBroadcastReceiver();
  //      IntentFilter intentFilter = new IntentFilter(Intent.ACTION_USER_PRESENT);

   //     intentFilter.addAction(Intent.ACTION_SCREEN_ON);
     //   intentFilter.addAction(Intent.ACTION_SCREEN_OFF);
	//getApplicationContext().registerReceiverAsUser(mReceiver, UserHandle.ALL, intentFilter, null, null);
//         System.out.println("unifyid registerReceiver!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//	registerReceiver(mReceiver, intentFilter);	
        // Example of a call to a native method
//        TextView tv = (TextView) findViewById(R.id.sample_text);
  //      tv.setText(stringFromJNI());
        // 获取传感器管理器
  //      sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
  //      this.sensorData = new SensorData();
    //    mSession = WindowManagerGlobal.getWindowSession();
   //     mWindow.setSession(mSession);

        // 获取全部传感器列表
//        List<Sensor> sensors = sensorManager.getSensorList(Sensor.TYPE_ALL);
//// 打印每个传感器信息
//        StringBuilder strLog = new StringBuilder();
//        int iIndex = 1;
//        for (Sensor item : sensors) {
//            strLog.append(iIndex + ".");
//            strLog.append(" Sensor Type - " + item.getType() + "\r\n");
//            strLog.append(" Sensor Name - " + item.getName() + "\r\n");
//            strLog.append(" Sensor Version - " + item.getVersion() + "\r\n");
//            strLog.append(" Sensor Vendor - " + item.getVendor() + "\r\n");
//            strLog.append(" Maximum Range - " + item.getMaximumRange() + "\r\n");
//            strLog.append(" Minimum Delay - " + item.getMinDelay() + "\r\n");
//            strLog.append(" Power - " + item.getPower() + "\r\n");
//            strLog.append(" Resolution - " + item.getResolution() + "\r\n");
//            strLog.append("\r\n");
//            iIndex++;
//        }
//        System.out.println(strLog.toString());
         /*
        try {

        //    mInputChannel = new InputChannel();
            mLayout.x = 0;
            mLayout.y = 0;
            mLayout.width = 1280;
            mLayout.height = 1920;
            mLayout.format = PixelFormat.RGBA_8888;
            mLayout.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
                    | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS
                    | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN
                    | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
            mLayout.memoryType = 0;
            mLayout.token = getWindow().getDecorView().getWindowToken();
            mSession.addToDisplay(mWindow, mWindow.mSeq, mLayout, View.VISIBLE, Display.DEFAULT_DISPLAY, mContentInsets, mStableInsets, mOutsets, mInputChannel);
            mInputEventReceiver = new UnifyIdInputEventReceiver(mInputChannel, Looper.myLooper());

        } catch (RemoteException ex) {

        }
        */
//        if ((sensorManager.getDefaultSensor(1) == null) || (sensorManager.getDefaultSensor(9) == null) || (sensorManager.getDefaultSensor(4) == null))
//        {
//            Toast.makeText(getApplicationContext(), "该机型传感器不完整，无法利用SensorDemo检测！ ", 0).show();
//        }
//        else {
//            this.touchDataList = new ArrayList<TouchData>();
//            this.touchDataList.clear();
//            //View globalView = findViewById(android.R.id.content);
//
//            getWindow().getDecorView().setOnTouchListener(new View.OnTouchListener() {
//
//                @Override
//                public boolean onTouch(View v, MotionEvent event) {
//                    long timestamp = SystemClock.elapsedRealtimeNanos();
//                    TouchData localTouchData = new TouchData();
//                    localTouchData.timestamp = timestamp;
//                    localTouchData.touchX = event.getX();
//                    localTouchData.touchY = event.getY();
//                    localTouchData.pressure = event.getPressure();
//                    MainActivity.this.touchDataList.add(localTouchData);
//                    switch(event.getAction()){
//                        case MotionEvent.ACTION_DOWN:
//			    if (MainActivity.this.screen_on) {
//				    MainActivity.this.screen_on = false;
//                                    MainActivity.this.sensorManager.unregisterListener(MainActivity.this);
//                                    System.out.println("---timerUp schedule");
//                                    writeAccTxt();
//                                    writeGyroTxt();
//                                    writeGravityTxt();
//                                    MainActivity.this.AccSensorDataList.clear();
//                                    MainActivity.this.GravitySensorDataList.clear();
//                                    MainActivity.this.GyroSensorDataList.clear();
//                                    MainActivity.this.isAppRunning = false;
//			    }
//                            System.out.println("---action down-----"+"x "+ event.getX() + " y "+ event.getY());
//                            saveData();
//                            //  show.setText("起始位置为："+"("+event.getX()+" , "+event.getY()+")");
//                            break;
//                        case MotionEvent.ACTION_MOVE:
//                            System.out.println("---action move-----");
//                            //  show.setText("移动中坐标为："+"("+event.getX()+" , "+event.getY()+")");
//                            break;
//                        case MotionEvent.ACTION_UP:
//                            System.out.println("---action up-----");
//                            MainActivity.this.writeTouchTxt();
//                            MainActivity.this.touchDataList.clear();
//                            if (MainActivity.this.timerUp == null)
//                                MainActivity.this.timerUp = new Timer();
//                                    MainActivity.this.sensorManager.unregisterListener(MainActivity.this);
//                                    System.out.println("---timerUp schedule");
//                                    writeAccTxt();
//                                    writeGyroTxt();
//                                    writeGravityTxt();
//                                    MainActivity.this.AccSensorDataList.clear();
//                                    MainActivity.this.GravitySensorDataList.clear();
//                                    MainActivity.this.GyroSensorDataList.clear();
//                                    MainActivity.this.isAppRunning = false;
//                    }
//                    return false;
//                }
//            });
//
//            this.sensorDataList = new ArrayList<SensorData>();
//            this.sensorDataList.clear();
//            this.GravitySensorDataList = new ArrayList<SensorData>();
//            this.GravitySensorDataList.clear();
//            this.AccSensorDataList = new ArrayList<SensorData>();
//            this.AccSensorDataList.clear();
//            this.GyroSensorDataList = new ArrayList<SensorData>();
//            this.GyroSensorDataList.clear();
//            // saveData();
//        }

    }



    //检测用户是否对本app开启了“Apps with usage access”权限
    private boolean hasPermission() {
        AppOpsManager appOps = (AppOpsManager)
                getSystemService(Context.APP_OPS_SERVICE);
        int mode = 0;
       // if (Build.VERSION.SDK_INT > Build.VERSION_CODES.KITKAT) {
            mode = appOps.checkOpNoThrow(AppOpsManager.OPSTR_GET_USAGE_STATS,
                    android.os.Process.myUid(), getPackageName());
    //    }
        return mode == AppOpsManager.MODE_ALLOWED;
    }

    private static final int MY_PERMISSIONS_REQUEST_PACKAGE_USAGE_STATS = 1101;
    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        if (requestCode == MY_PERMISSIONS_REQUEST_PACKAGE_USAGE_STATS) {
            if (!hasPermission()) {
                //若用户未开启权限，则引导用户开启“Apps with usage access”权限
                startActivityForResult(
                        new Intent(Settings.ACTION_USAGE_ACCESS_SETTINGS),
                        MY_PERMISSIONS_REQUEST_PACKAGE_USAGE_STATS);
            }
        }
    }

    public void onAccuracyChanged(Sensor paramSensor, int paramInt) {}
    public boolean isScreenOn()
    {
        PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
        return pm.isScreenOn();
    }

    private void saveData()
    {
        this.isAppRunning = true;
        this.sensorManager.registerListener(this, this.sensorManager.getDefaultSensor(1), SensorManager.SENSOR_DELAY_FASTEST);
        this.sensorManager.registerListener(this, this.sensorManager.getDefaultSensor(4), SensorManager.SENSOR_DELAY_FASTEST);
        this.sensorManager.registerListener(this, this.sensorManager.getDefaultSensor(9), SensorManager.SENSOR_DELAY_FASTEST);
        this.sensorManager.registerListener(this, this.sensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE), SensorManager.SENSOR_DELAY_FASTEST);
        /*
        this.timer = new Timer();
        this.timer.schedule(new TimerTask()
        {
            public void run()
            {
                Log.i("save run ", ""+ MainActivity.this.sensorDataList.size());
                while ((MainActivity.this.sensorDataList.size() < MainActivity.this.num) && (MainActivity.this.isScreenOn()) && (MainActivity.this.isAppRunning))
                {
                    Log.i("save ", ""+ MainActivity.this.sensorDataList.size());
                    SensorData localSensorData = new SensorData();
                    localSensorData.accelerometerTimeStamp = MainActivity.this.sensorData.accelerometerTimeStamp;
                    localSensorData.accelerometerX = MainActivity.this.sensorData.accelerometerX;
                    localSensorData.accelerometerY = MainActivity.this.sensorData.accelerometerY;
                    localSensorData.accelerometerZ = MainActivity.this.sensorData.accelerometerZ;
                    localSensorData.gravityTimeStamp = MainActivity.this.sensorData.gravityTimeStamp;
                    localSensorData.gravityX = MainActivity.this.sensorData.gravityX;
                    localSensorData.gravityY = MainActivity.this.sensorData.gravityY;
                    localSensorData.gravityZ = MainActivity.this.sensorData.gravityZ;
                    localSensorData.gyroscopeTimeStamp = MainActivity.this.sensorData.gyroscopeTimeStamp;
                    localSensorData.gyroscopeX = MainActivity.this.sensorData.gyroscopeX;
                    localSensorData.gyroscopeY = MainActivity.this.sensorData.gyroscopeY;
                    localSensorData.gyroscopeZ = MainActivity.this.sensorData.gyroscopeZ;
                    localSensorData.pressureTimeStamp = MainActivity.this.sensorData.pressureTimeStamp;
                    localSensorData.pressure = MainActivity.this.sensorData.pressure;
                    MainActivity.this.sensorDataList.add(localSensorData);
                    try
                    {
                        Thread.sleep(MainActivity.this.interval);
                    }
                    catch (Exception localException1)
                    {
                        Log.e("CollectDataService", localException1.getMessage());
                    }
                }
                MainActivity.this.writeTxt();
                Object localObject;
                if (MainActivity.this.pointNum < 100)
                {
                    Log.e("pointNum", "less than 100");

                }
                MainActivity.this.sensorDataList.clear();

            }
        }, 0L, 10000);*/
        return;
    }

    public void writeGravityTxt()
    {
        Object localdir1 = new File(Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData");
        if (!((File)localdir1).exists()) {
            ((File)localdir1).mkdir();
        }
        SensorData localSensorData;
        try
        {
            this.pointNum = 0;
            String localfile1 = new String();
            localfile1 = Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData/" + "gravity-" + this.sampleTimestamp + ".txt";
            this.FilePath = localfile1;
            Object file = new File(this.FilePath);
            FileWriter localObject1 = new FileWriter(this.FilePath, true);
            Iterator localObject2 = this.GravitySensorDataList.iterator();
            for (;((Iterator)localObject2).hasNext();)
            {
                localSensorData = (SensorData)((Iterator)localObject2).next();
                if ((Math.abs(localSensorData.gravityX) < 0.1D) && (Math.abs(localSensorData.gravityY) < 0.1D) && (Math.abs(localSensorData.gravityZ) < 0.1D))
                {
                    Log.e("device", "error");
                    // continue;
                }


                if ((Math.abs(localSensorData.gravityX) < 1.5D) && (Math.abs(localSensorData.gravityY) < 1.5D) && (Math.abs(localSensorData.gravityZ) > 9.0F))
                {
                    Log.e("device", "flat");
                    //  continue;
                }

                if ((localSensorData.gravityZ <= 0.0F) || (localSensorData.gravityZ >= 10.0F) || (localSensorData.gravityY <= 0.0F) || (localSensorData.gravityY >= 10.0F) || (localSensorData.gravityX <= -7.07D) || (localSensorData.gravityX >= 7.07D))
                {
                    Log.e("device", "octant error");
                    //  continue;
                }
                //this.pointNum += 1;
                localObject1.write( localSensorData.gravityTimeStamp + "\n" + localSensorData.gravityX + "\n" + localSensorData.gravityY + "\n" + localSensorData.gravityZ + "\n" + "\n");

            }
            localObject1.close();
        }
        catch (IOException localIOException)
        {
            localIOException.printStackTrace();
        }

    }

    public void writeGyroTxt()
    {
        Object localdir1 = new File(Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData");
        if (!((File)localdir1).exists()) {
            ((File)localdir1).mkdir();
        }
        SensorData localSensorData;
        try
        {
            this.pointNum = 0;
            String localfile1 = new String();
            localfile1 = Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData/" + "gyro-" + this.sampleTimestamp + ".txt";
            this.FilePath = localfile1;
            Object file = new File(this.FilePath);
            FileWriter localObject1 = new FileWriter(this.FilePath, true);
            Iterator localObject2 = this.GyroSensorDataList.iterator();
            for (;((Iterator)localObject2).hasNext();)
            {
                localSensorData = (SensorData)((Iterator)localObject2).next();

                if ((localSensorData.gyroscopeX == 0.0F) && (localSensorData.gyroscopeY == 0.0F) && (localSensorData.gyroscopeZ == 0.0F))
                {
                    Log.e("device", "exception");
                    //   continue;
                }

                //this.pointNum += 1;
                localObject1.write( localSensorData.gyroscopeTimeStamp + "\n" + localSensorData.gyroscopeX + "\n" + localSensorData.gyroscopeY + "\n" + localSensorData.gyroscopeZ + "\n" + "\n");

            }
            localObject1.close();
        }
        catch (IOException localIOException)
        {
            localIOException.printStackTrace();
        }

    }

    public void writeAccTxt()
    {
        Object localdir1 = new File(Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData");
        if (!((File)localdir1).exists()) {
            ((File)localdir1).mkdir();
        }
        SensorData localSensorData;
        try
        {
            this.pointNum = 0;
            String localfile1 = new String();
            localfile1 = Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData/" + "acc-" + this.sampleTimestamp + ".txt";
            this.FilePath = localfile1;
            Object file = new File(this.FilePath);
            FileWriter localObject1 = new FileWriter(this.FilePath, true);
            Iterator localObject2 = this.AccSensorDataList.iterator();
            for (;((Iterator)localObject2).hasNext();)
            {
                localSensorData = (SensorData)((Iterator)localObject2).next();

                //this.pointNum += 1;
                localObject1.write( localSensorData.accelerometerTimeStamp + "\n" + localSensorData.accelerometerX + "\n" + localSensorData.accelerometerY + "\n" + localSensorData.accelerometerZ + "\n" + "\n");

            }
            localObject1.close();
        }
        catch (IOException localIOException)
        {
            localIOException.printStackTrace();
        }

    }

    public void writeTxt()
    {
        Object localdir1 = new File(Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData");
        if (!((File)localdir1).exists()) {
            ((File)localdir1).mkdir();
        }
        SensorData localSensorData;
        try
        {
            this.pointNum = 0;
            String localfile1 = new String();
            localfile1 = Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData/" + this.sampleTimestamp + ".txt";
            this.FilePath = localfile1;
            Object file = new File(this.FilePath);
            FileWriter localObject1 = new FileWriter(this.FilePath, true);
            Iterator localObject2 = this.sensorDataList.iterator();
            for (;((Iterator)localObject2).hasNext();)
            {
                localSensorData = (SensorData)((Iterator)localObject2).next();
                if ((Math.abs(localSensorData.gravityX) < 0.1D) && (Math.abs(localSensorData.gravityY) < 0.1D) && (Math.abs(localSensorData.gravityZ) < 0.1D))
                {
                    Log.e("device", "error");
                    // continue;
                }


                if ((Math.abs(localSensorData.gravityX) < 1.5D) && (Math.abs(localSensorData.gravityY) < 1.5D) && (Math.abs(localSensorData.gravityZ) > 9.0F))
                {
                    Log.e("device", "flat");
                    //  continue;
                }
                if ((localSensorData.gyroscopeX == 0.0F) && (localSensorData.gyroscopeY == 0.0F) && (localSensorData.gyroscopeZ == 0.0F))
                {
                    Log.e("device", "exception");
                    //   continue;
                }
                if ((localSensorData.gravityZ <= 0.0F) || (localSensorData.gravityZ >= 10.0F) || (localSensorData.gravityY <= 0.0F) || (localSensorData.gravityY >= 10.0F) || (localSensorData.gravityX <= -7.07D) || (localSensorData.gravityX >= 7.07D))
                {
                    Log.e("device", "octant error");
                    //  continue;
                }
                this.pointNum += 1;
                localObject1.write( localSensorData.accelerometerTimeStamp + "\n" + localSensorData.accelerometerX + "\n" + localSensorData.accelerometerY + "\n" + localSensorData.accelerometerZ + "\n" + localSensorData.gyroscopeTimeStamp + "\n" + localSensorData.gyroscopeX + "\n" + localSensorData.gyroscopeY + "\n" + localSensorData.gyroscopeZ + "\n" + localSensorData.gravityTimeStamp + "\n" + localSensorData.gravityX + "\n" + localSensorData.gravityY + "\n" + localSensorData.gravityZ + "\n"+"\n");

            }
            localObject1.close();
        }
        catch (IOException localIOException)
        {
            localIOException.printStackTrace();
        }

    }

    public void writeTouchTxt()
    {
        this.sampleTimestamp = System.currentTimeMillis();
        Object localdir1 = new File(Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData");
        if (!((File)localdir1).exists()) {
            ((File)localdir1).mkdir();
        }
        TouchData localTouchData;
        try
        {
            String localfile1 = new String();
            localfile1 = Environment.getExternalStorageDirectory().getPath() + "/SensorUnifyIdData/" + "touch-" + this.sampleTimestamp + ".txt";
            this.FilePath = localfile1;
            Object file = new File(this.FilePath);
            FileWriter localObject1 = new FileWriter(this.FilePath, true);
            Iterator localObject2 = this.touchDataList.iterator();
            for (;((Iterator)localObject2).hasNext();)
            {
                localTouchData = (TouchData)((Iterator)localObject2).next();
                localObject1.write(localTouchData.timestamp + "\n" + localTouchData.pressure + "\n" + localTouchData.touchX + "\n" + localTouchData.touchY + "\n" + "\n");

            }
            localObject1.close();
        }
        catch (IOException localIOException)
        {
            localIOException.printStackTrace();
        }

    }

    public void onSensorChanged(SensorEvent paramSensorEvent)
    {
        SensorData localSensorData = new SensorData();

        long timestamp = System.currentTimeMillis();
        if (paramSensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            Log.d("acc x: ", "" + paramSensorEvent.values[0]);
             Log.d("acc y: ", "" + paramSensorEvent.values[1]);
              Log.d("acc z: ", "" + paramSensorEvent.values[2]);
             Log.d("acc t: ", "" + paramSensorEvent.timestamp);
            localSensorData.accelerometerTimeStamp = paramSensorEvent.timestamp;
            //  this.sensorData.accelerometerTimeStamp = timestamp;
            localSensorData.accelerometerX = paramSensorEvent.values[0];
            localSensorData.accelerometerY = paramSensorEvent.values[1];
            localSensorData.accelerometerZ = paramSensorEvent.values[2];
            this.AccSensorDataList.add(localSensorData);
        } else if (paramSensorEvent.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
            //  Log.d("gyr x: ", "" + paramSensorEvent.values[0]);
            // Log.d("gyr y: ", "" + paramSensorEvent.values[1]);
            //  Log.d("gyr z: ", "" + paramSensorEvent.values[2]);
            localSensorData.gyroscopeTimeStamp = paramSensorEvent.timestamp;
            // this.sensorData.gyroscopeTimeStamp = timestamp;
            localSensorData.gyroscopeX = paramSensorEvent.values[0];
            localSensorData.gyroscopeY = paramSensorEvent.values[1];
            localSensorData.gyroscopeZ = paramSensorEvent.values[2];
            this.GyroSensorDataList.add(localSensorData);
        } else if (paramSensorEvent.sensor.getType() == Sensor.TYPE_GRAVITY) {
            //  Log.d("gravity x: ", "" + paramSensorEvent.values[0]);
            //  Log.d("gravity y: ", "" + paramSensorEvent.values[1]);
            //   Log.d("gravity z: ", "" + paramSensorEvent.values[2]);
            localSensorData.gravityTimeStamp = paramSensorEvent.timestamp;
            //  this.sensorData.gravityTimeStamp = timestamp;
            localSensorData.gravityX = paramSensorEvent.values[0];
            localSensorData.gravityY = paramSensorEvent.values[1];
            localSensorData.gravityZ = paramSensorEvent.values[2];
            this.GravitySensorDataList.add(localSensorData);
        } else if (paramSensorEvent.sensor.getType() == Sensor.TYPE_PRESSURE) {
            localSensorData.pressureTimeStamp = paramSensorEvent.timestamp;
            // this.sensorData.pressureTimeStamp = timestamp;
            localSensorData.pressure = paramSensorEvent.values[0];
        }

        //MainActivity.this.sensorDataList.add(this.sensorData);

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();
}
