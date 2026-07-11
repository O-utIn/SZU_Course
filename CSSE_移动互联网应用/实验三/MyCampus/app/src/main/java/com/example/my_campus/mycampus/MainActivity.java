package com.example.my_campus.mycampus;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

public class MainActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 判断是否是手机布局（包含底部导航按钮）
        Button btnNews = findViewById(R.id.btn_nav_news);
        Button btnServices = findViewById(R.id.btn_nav_services);

        if (btnNews != null && btnServices != null) {
            // 默认加载左侧新闻 Fragment
            if (savedInstanceState == null) {
                replaceFragment(new LeftFragment());
            }

            // 点击“首页新闻”切换
            btnNews.setOnClickListener(v -> {
                btnNews.setTextColor(android.graphics.Color.parseColor("#8A0025")); // 深大红
                btnServices.setTextColor(android.graphics.Color.parseColor("#333333"));
                replaceFragment(new LeftFragment());
            });

            // 点击“服务大厅”切换
            btnServices.setOnClickListener(v -> {
                btnServices.setTextColor(android.graphics.Color.parseColor("#8A0025"));
                btnNews.setTextColor(android.graphics.Color.parseColor("#333333"));
                replaceFragment(new RightFragment());
            });
        }
    }

    // 切换 Fragment 的通用方法 (教材第4章知识点)
    private void replaceFragment(Fragment fragment) {
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction transaction = fragmentManager.beginTransaction();
        transaction.replace(R.id.fragment_container, fragment);
        transaction.commit();
    }

    // 预留的强制下线测试方法
    public void simulateNetworkError() {
        Intent intent = new Intent("com.example.my_campus.FORCE_OFFLINE");
        intent.setPackage(getPackageName());
        sendBroadcast(intent);
    }
}