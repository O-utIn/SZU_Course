package com.example.my_campus.mycampus;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

public class BaseActivity extends AppCompatActivity {
    private ForceOfflineReceiver receiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityCollector.addActivity(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction("com.example.my_campus.FORCE_OFFLINE");
        receiver = new ForceOfflineReceiver();
        androidx.core.content.ContextCompat.registerReceiver(
                this,
                receiver,
                intentFilter,
                androidx.core.content.ContextCompat.RECEIVER_NOT_EXPORTED
        );
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (receiver != null) {
            unregisterReceiver(receiver);
            receiver = null;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ActivityCollector.removeActivity(this);
    }

    // 内部类：强制下线广播接收器
    static class ForceOfflineReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(final Context context, Intent intent) {
            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            builder.setTitle("警告");
            builder.setMessage("您的账号在异地登录或网络异常，请重新登录！");
            builder.setCancelable(false);
            builder.setPositiveButton("确定", (dialog, which) -> {
                ActivityCollector.finishAll(); // 销毁所有活动
                Intent i = new Intent(context, LoginActivity.class);
                context.startActivity(i); // 重新启动登录界面
            });
            builder.show();
        }
    }
}