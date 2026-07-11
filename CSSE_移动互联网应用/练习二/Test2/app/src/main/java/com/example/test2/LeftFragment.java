package com.example.test2;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class LeftFragment extends Fragment {

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        // 加载左边碎片的布局
        View view = inflater.inflate(R.layout.fragment_left, container, false);

        Button btnSendMessage = view.findViewById(R.id.btn_send_message);
        btnSendMessage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // ====== 书本 P151 的核心通信逻辑 ======

                // 1. 获取当前碎片所在的 Activity 实例
                MainActivity activity = (MainActivity) getActivity();

                if (activity != null) {
                    // 2. 通过 Activity 的 FragmentManager 去寻找右边碎片的实例
                    // (注意：这里使用 getSupportFragmentManager() 替代老版本的 getFragmentManager())
                    RightFragment rightFragment = (RightFragment) activity.getSupportFragmentManager()
                            .findFragmentById(R.id.right_fragment);

                    // 3. 找到右边碎片后，直接调用它的 public 方法传递数据
                    if (rightFragment != null) {
                        rightFragment.updateText("你好！我是左边碎片发来的消息。");
                    }
                }
            }
        });

        return view;
    }
}