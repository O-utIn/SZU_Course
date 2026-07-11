package com.example.my_campus.mycampus;

import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import androidx.fragment.app.Fragment;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

public class LeftFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // 需自行创建 fragment_left.xml (包含一个通知文本框和一个下线测试按钮)
        View view = inflater.inflate(R.layout.fragment_left, container, false);

        Button testOfflineBtn = view.findViewById(R.id.btn_test_offline);
        testOfflineBtn.setOnClickListener(v -> {
            ((MainActivity) getActivity()).simulateNetworkError();
        });

        // 数据持久化技术二：文件存储 (适用场景：缓存大段文本/重要通知)
        saveNoticeToFile("【重要通知】深大2025-2026学年校历发布...");

        return view;
    }

    private void saveNoticeToFile(String inputText) {
        FileOutputStream out = null;
        BufferedWriter writer = null;
        try {
            out = getActivity().openFileOutput("notice_cache", Context.MODE_PRIVATE);
            writer = new BufferedWriter(new OutputStreamWriter(out));
            writer.write(inputText);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (writer != null) writer.close();
            } catch (Exception e) { e.printStackTrace(); }
        }
    }
}