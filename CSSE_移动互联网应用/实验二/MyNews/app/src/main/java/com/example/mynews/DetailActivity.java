package com.example.mynews;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class DetailActivity extends AppCompatActivity {
    private TextView tvContent;
    private String targetUrl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detail);

        TextView tvTitle = findViewById(R.id.detail_title);
        tvContent = findViewById(R.id.tv_content);
        Button btnBrowser = findViewById(R.id.btn_browser);
        Button btnDownload = findViewById(R.id.btn_download);

        // 接收上一级传来的数据
        String title = getIntent().getStringExtra("title");
        targetUrl = getIntent().getStringExtra("url");
        tvTitle.setText(title);

        // 方案A：隐式Intent调用系统浏览器浏览
        btnBrowser.setOnClickListener(v -> {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(Uri.parse(targetUrl));
            startActivity(intent);
        });

        // 方案B：HttpURLConnection 下载网页源码到本地TextView展示
        btnDownload.setOnClickListener(v -> sendRequestWithHttpURLConnection());
    }

    private void sendRequestWithHttpURLConnection() {
        tvContent.setText("正在加载...");
        new Thread(() -> {
            HttpURLConnection connection = null;
            BufferedReader reader = null;
            try {
                URL url = new URL(targetUrl);
                connection = (HttpURLConnection) url.openConnection();
                connection.setRequestMethod("GET");
                connection.setConnectTimeout(8000);
                connection.setReadTimeout(8000);
                InputStream in = connection.getInputStream();
                reader = new BufferedReader(new InputStreamReader(in));
                StringBuilder response = new StringBuilder();
                String line;
                while ((line = reader.readLine()) != null) {
                    response.append(line).append("\n");
                }
                showResponse(response.toString());
            } catch (Exception e) {
                e.printStackTrace();
                showResponse("加载失败: " + e.getMessage());
            } finally {
                if (reader != null) {
                    try { reader.close(); } catch (Exception e) { e.printStackTrace(); }
                }
                if (connection != null) { connection.disconnect(); }
            }
        }).start();
    }

    // UI更新在主线程
    private void showResponse(final String response) {
        runOnUiThread(() -> tvContent.setText(response));
    }
}