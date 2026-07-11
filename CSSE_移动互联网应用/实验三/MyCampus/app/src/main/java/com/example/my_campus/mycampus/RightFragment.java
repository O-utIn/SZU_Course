package com.example.my_campus.mycampus;

import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.fragment.app.Fragment;

public class RightFragment extends Fragment {
    private MyDatabaseHelper dbHelper;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // 需自行创建 fragment_right.xml
        View view = inflater.inflate(R.layout.fragment_right, container, false);
        TextView serviceListText = view.findViewById(R.id.tv_service_list);

        dbHelper = new MyDatabaseHelper(getActivity(), "CampusStore.db", null, 1);

        // 数据持久化技术三：SQLite读取 (适用场景：结构化强、需频繁增删改查的大量数据)
        loadServicesFromDB(serviceListText);

        return view;
    }

    private void loadServicesFromDB(TextView textView) {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        Cursor cursor = db.query("Service", null, null, null, null, null, null);
        StringBuilder builder = new StringBuilder();
        if (cursor.moveToFirst()) {
            do {
                String category = cursor.getString(cursor.getColumnIndexOrThrow("category"));
                String name = cursor.getString(cursor.getColumnIndexOrThrow("name"));
                builder.append("[").append(category).append("] - ").append(name).append("\n");
            } while (cursor.moveToNext());
        }
        cursor.close();
        textView.setText(builder.toString());
        // 实际开发中，这里应将数据传给 RecyclerView.Adapter 进行网格展示
    }
}