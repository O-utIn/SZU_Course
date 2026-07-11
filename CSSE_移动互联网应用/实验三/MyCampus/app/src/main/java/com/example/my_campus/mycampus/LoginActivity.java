package com.example.my_campus.mycampus;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

public class LoginActivity extends BaseActivity {
    private SharedPreferences pref;
    private SharedPreferences.Editor editor;
    private EditText accountEdit, passwordEdit;
    private CheckBox rememberPass;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login); // 需自行创建简单的登录XML布局

        pref = PreferenceManager.getDefaultSharedPreferences(this);
        accountEdit = findViewById(R.id.account);
        passwordEdit = findViewById(R.id.password);
        rememberPass = findViewById(R.id.remember_pass);
        Button loginButton = findViewById(R.id.login);

        // 检查是否记住了密码
        boolean isRemember = pref.getBoolean("remember_password", false);
        if (isRemember) {
            accountEdit.setText(pref.getString("account", ""));
            passwordEdit.setText(pref.getString("password", ""));
            rememberPass.setChecked(true);
        }

        loginButton.setOnClickListener(v -> {
            String account = accountEdit.getText().toString();
            String password = passwordEdit.getText().toString();
            // 简单模拟登录验证
            if (account.equals("admin") && password.equals("123456")) {
                editor = pref.edit();
                if (rememberPass.isChecked()) {
                    editor.putBoolean("remember_password", true);
                    editor.putString("account", account);
                    editor.putString("password", password);
                } else {
                    editor.clear();
                }
                editor.apply(); // 持久化技术一：SharedPreferences 适用场景：简单配置

                Intent intent = new Intent(LoginActivity.this, MainActivity.class);
                startActivity(intent);
                finish();
            } else {
                Toast.makeText(LoginActivity.this, "账号或密码错误", Toast.LENGTH_SHORT).show();
            }
        });
    }
}