package Part1;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class FileWrite {

    public static void main(String[] args) {
        String watchesFilePath = "E:\\Programs\\JAVA\\Lab4\\src\\Part1\\Watches.txt";
        String reviewFilePath = "E:\\Programs\\JAVA\\Lab4\\src\\Part1\\review.txt";

        List<ReviewRecord> reviewRecords = new ArrayList<>();

        try (BufferedReader reader = new BufferedReader(new FileReader(watchesFilePath))) {
            String line;
            ReviewRecord currentRecord = null;
            while ((line = reader.readLine())!= null) {
                if (line.startsWith("product/productId:")) {
                    // 开始处理新的记录，先保存上一个记录（如果有）
                    if (currentRecord!= null) {
                        reviewRecords.add(currentRecord);
                    }
                    currentRecord = new ReviewRecord();
                    currentRecord.productId = line.split(": ")[1];
                } else if (line.startsWith("review/userId:")) {
                    currentRecord.userId = line.split(": ")[1];
                }
            }
            // 处理最后一个记录
            if (currentRecord!= null) {
                reviewRecords.add(currentRecord);
            }

            // 删除ID为unknown的记录
            reviewRecords.removeIf(record -> "unknown".equals(record.productId) || "unknown".equals(record.userId));

            // 按照要求排序
            Collections.sort(reviewRecords, (r1, r2) -> {
                int userIdComparison = r1.userId.compareTo(r2.userId);
                if (userIdComparison == 0) {
                    return r1.productId.compareTo(r2.productId);
                }
                return userIdComparison;
            });

            // 写入review.txt文件
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(reviewFilePath))) {
                writer.write("userID;productID");
                writer.newLine();

                for (ReviewRecord record : reviewRecords) {
                    writer.write(record.userId + ";" + record.productId);
                    writer.newLine();
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // 内部类用于存储每条记录的信息
    private static class ReviewRecord {
        String productId;
        String userId;
    }
}
