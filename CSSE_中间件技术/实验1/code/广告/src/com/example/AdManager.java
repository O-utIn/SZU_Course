package com.example;
import com.example.Ad;
import java.util.ArrayList;
import java.util.List;

public class AdManager {
    private static List<com.example.Ad> ads = new ArrayList<>();

    // 添加广告
    public static void addAd(String imagePath, String linkUrl) {
        ads.add(new Ad(imagePath, linkUrl));
    }

    // 获取所有广告
    public static List<Ad> getAds() {
        return ads;
    }

    // 获取随机广告
    public static Ad getRandomAd() {
        if (ads.isEmpty()) return null;
        int index = (int) (Math.random() * ads.size());
        return ads.get(index);
    }
}
