package com.example.mynews;

public class PreferenceItem {
    private String title;
    private String source;
    private String time;
    private String url; // 用于网络请求跳转

    public PreferenceItem(String title, String source, String time, String url) {
        this.title = title;
        this.source = source;
        this.time = time;
        this.url = url;
    }
    // Getter方法
    public String getTitle() { return title; }
    public String getSource() { return source; }
    public String getTime() { return time; }
    public String getUrl() { return url; }
}