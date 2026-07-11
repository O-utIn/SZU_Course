package com.example.mynews;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.viewpager2.adapter.FragmentStateAdapter;
import androidx.viewpager2.widget.ViewPager2;
import com.google.android.material.tabs.TabLayout;
import com.google.android.material.tabs.TabLayoutMediator;
import androidx.fragment.app.Fragment;
import androidx.annotation.NonNull;

public class MainActivity extends AppCompatActivity {
    // 满足要求2：个人3个方面的信息菜单
    private final String[] titles = new String[]{"科技控", "旅行记", "读书角"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TabLayout tabLayout = findViewById(R.id.tabLayout);
        ViewPager2 viewPager = findViewById(R.id.viewPager);

        viewPager.setAdapter(new FragmentStateAdapter(this) {
            @NonNull
            @Override
            public Fragment createFragment(int position) {
                return new ListFragment(titles[position]);
            }
            @Override
            public int getItemCount() { return titles.length; }
        });

        // 将 TabLayout 与 ViewPager2 联动
        new TabLayoutMediator(tabLayout, viewPager,
                (tab, position) -> tab.setText(titles[position])
        ).attach();
    }
}