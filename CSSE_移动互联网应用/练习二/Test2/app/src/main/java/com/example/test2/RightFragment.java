package com.example.test2;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class RightFragment extends Fragment {

    private TextView tvReceiveMessage;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        // 加载右边碎片的布局
        View view = inflater.inflate(R.layout.fragment_right, container, false);
        // 绑定 TextView 控件
        tvReceiveMessage = view.findViewById(R.id.tv_receive_message);
        return view;
    }

    /**
     * 这是一个公开(public)的方法，供左边碎片通过Activity调用
     */
    public void updateText(String text) {
        if (tvReceiveMessage != null) {
            tvReceiveMessage.setText(text);
        }
    }
}