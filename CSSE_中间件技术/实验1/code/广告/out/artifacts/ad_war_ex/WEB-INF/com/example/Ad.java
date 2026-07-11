package com.example;

public class Ad {
    private String imagePath;  // 图片路径
    private String linkUrl;    // 链接地址

    public Ad(String imagePath, String linkUrl) {
        this.imagePath = imagePath;
        this.linkUrl = linkUrl;
    }

    public String getImagePath() {
        return imagePath;
    }

    public String getLinkUrl() {
        return linkUrl;
    }
}
