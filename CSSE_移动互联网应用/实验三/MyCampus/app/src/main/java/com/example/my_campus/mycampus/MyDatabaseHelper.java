package com.example.my_campus.mycampus;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class MyDatabaseHelper extends SQLiteOpenHelper {
    public static final String CREATE_SERVICE = "create table Service ("
            + "id integer primary key autoincrement, "
            + "category text, " // 如：教师事务、学生事务
            + "name text)";    // 如：办事大厅、UOOC课程

    private Context mContext;

    public MyDatabaseHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);
        mContext = context;
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(CREATE_SERVICE);
        // 初始化一些模拟数据
        db.execSQL("insert into Service (category, name) values ('教师事务', '办事大厅')");
        db.execSQL("insert into Service (category, name) values ('学生事务', 'UOOC课程')");
        db.execSQL("insert into Service (category, name) values ('网上服务', '图书馆')");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        // 升级逻辑
    }
}