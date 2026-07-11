package com.example.mynews;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import java.util.ArrayList;
import java.util.List;

public class ListFragment extends Fragment {
    private String category;

    public ListFragment(String category) {
        this.category = category;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_list, container, false); // 需要创建一个简单的包含RecyclerView的布局
        RecyclerView recyclerView = view.findViewById(R.id.recyclerView);

        LinearLayoutManager layoutManager = new LinearLayoutManager(getContext());
        recyclerView.setLayoutManager(layoutManager);

        // 模拟生成不同方面的个人偏好数据
        List<PreferenceItem> itemList = new ArrayList<>();
        if ("科技控".equals(category)) {
            itemList.add(new PreferenceItem("Android 14 新特性详解", "CSDN", "2小时前", "https://developer.android.google.cn/"));
            itemList.add(new PreferenceItem("AI大模型发展趋势分析", "科技博客", "5小时前", "https://www.bing.com/"));
        } else if ("旅行记".equals(category)) {
            itemList.add(new PreferenceItem("西藏自驾游全攻略", "携程游记", "1天前", "https://www.ctrip.com/"));
            itemList.add(new PreferenceItem("寻找隐秘的江南水乡", "马蜂窝", "3天前", "https://www.mafengwo.cn/"));
        } else if ("读书角".equals(category)) {
            itemList.add(new PreferenceItem("《第一行代码》读书笔记", "个人博客", "10分钟前", "https://github.com/"));
            itemList.add(new PreferenceItem("年度十佳科幻小说推荐", "豆瓣阅读", "1周前", "https://book.douban.com/"));
        }

        ItemAdapter adapter = new ItemAdapter(itemList);
        recyclerView.setAdapter(adapter);
        return view;
    }
}