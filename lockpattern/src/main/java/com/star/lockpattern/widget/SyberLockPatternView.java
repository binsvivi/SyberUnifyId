package com.star.lockpattern.widget;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.util.AttributeSet;
import android.view.HapticFeedbackConstants;
import android.view.MotionEvent;
import android.view.View;

import com.star.lockpattern.R;
import com.star.lockpattern.util.LockPatternUtil;
import com.syber.internal.widget.UnifyIdApp;
import com.syber.internal.widget.UnifyIdHelper;

import java.util.ArrayList;
import java.util.List;

/**
 * nine block box lock
 * @author Sym
 */
public class SyberLockPatternView extends LockPatternView {


    private static UnifyIdHelper mUnifyIdHelper;

	private static final String TAG = "SyberLockPatternView";

	public SyberLockPatternView(Context context) {
		this(context, null);
	}

	public SyberLockPatternView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public SyberLockPatternView(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		if (mUnifyIdHelper == null) {
			mUnifyIdHelper = UnifyIdApp.getInstance();
		}

	}


	@SuppressLint("ClickableViewAccessibility")
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (mUnifyIdHelper != null)
			mUnifyIdHelper.onPointerEvent(event);
		return super.onTouchEvent(event);
	}

	
}
