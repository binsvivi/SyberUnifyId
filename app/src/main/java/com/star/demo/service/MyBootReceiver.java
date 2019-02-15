package com.star.demo.service;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

public class MyBootReceiver extends BroadcastReceiver{

	private static final String TAG="MyBootReceiver";
	/*在setPackageStoppedState之后 判断是否还执行开机自启动操作*/
	private static final String action_boot="android.intent.action.BOOT_COMPLETED";
	private static final String action_restart_check_app="com.syberos.checking_app.destroy";
	private static final String action_restart_check_update="com.syberos.checking_update.destroy";
	private static final String android_location_service="com.syberos.location_refresh.service";
	private static final int EVENT_TYPE_ANDROID_LOCATION=125;
	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		 if (intent.getAction().equals(action_boot)){
	           Log.v(TAG,"start action_boot");
		   {
	           Intent it=new Intent(context,UnifyIdService.class);
          	   it.setAction(UnifyIdService.REQUEST_UNIFYID_ACTION);
          	   context.startService(it);
		   }
		   {
	           Intent it=new Intent(context,CheckUpdateService.class);
          	   it.setAction(CheckUpdateService.REQUEST_UPDATE_DATA_ACTION);
          	   context.startService(it);
		   }
	     }

		if (intent.getAction().equals(action_restart_check_app)){
			Log.v(TAG,"start action_boot");
			{
				Intent it=new Intent(context,CheckingAppRunningService.class);
				//it.setAction(UnifyIdService.REQUEST_UNIFYID_ACTION);
				context.startService(it);
			}

		}

		if (intent.getAction().equals(action_restart_check_update)){
			Log.v(TAG,"start action_check_update");
			{
				Intent it=new Intent(context,CheckUpdateService.class);
				//it.setAction(UnifyIdService.REQUEST_UNIFYID_ACTION);
				context.startService(it);
			}

		}
	}
}
