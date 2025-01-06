package Part1;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class CalSort {

    // 用于存储每个产品关联的用户集合
    private static Map<String, List<String>> productUserMap = new HashMap<>();

    public static void main(String[] args) {
        String reviewFilePath = "E:\\Programs\\JAVA\\Lab4\\src\\Part1\\review.txt";
        String productNeighborhoodFilePath = "E:\\Programs\\JAVA\\Lab4\\src\\Part1\\productNeighborhood.txt";

        try (BufferedReader reader = new BufferedReader(new FileReader(reviewFilePath))) {
            String line;
            while ((line = reader.readLine())!= null) {
                if (!line.startsWith("userID;productID")) {
                    String[] parts = line.split(";");
                    String userId = parts[0];
                    String productId = parts[1];

                    productUserMap.putIfAbsent(productId, new ArrayList<>());
                    productUserMap.get(productId).add(userId);
                }
            }

            // 获取所有产品ID
            List<String> productIds = new ArrayList<>(productUserMap.keySet());

            // 创建线程池
            ExecutorService executorService = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());

            // 用于存储每个产品与其他产品的相似度结果
            Map<String, List<SimilarityResult>> similarityResults = new HashMap<>();

            // 计算每个产品与其他产品的相似度
            for (String productId : productIds) {
                executorService.submit(() -> {
                    List<SimilarityResult> resultList = new ArrayList<>();
                    for (String otherProductId : productIds) {
                        if (!productId.equals(otherProductId)) {
                            double similarity = calculateJaccardIndex(productUserMap.get(productId),
                                    productUserMap.get(otherProductId));
                            resultList.add(new SimilarityResult(otherProductId, similarity));
                        }
                    }
                    similarityResults.put(productId, resultList);
                });
            }

            // 关闭线程池
            executorService.shutdown();
            try {
                executorService.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            // 对每个产品的相似度结果进行排序，并选取前3个
            List<ProductNeighborhoodResult> neighborhoodResults = new ArrayList<>();
            for (Map.Entry<String, List<SimilarityResult>> entry : similarityResults.entrySet()) {
                String productId = entry.getKey();
                List<SimilarityResult> results = entry.getValue();
                results.sort((r1, r2) -> {
                    if (r1.similarity == r2.similarity) {
                        return r1.productId.compareTo(r2.productId);
                    }
                    return Double.compare(r2.similarity, r1.similarity);
                });
                List<String> topThreeProductIds = new ArrayList<>();
                for (int i = 0; i < Math.min(3, results.size()); i++) {
                    topThreeProductIds.add(results.get(i).productId);
                }
                neighborhoodResults.add(new ProductNeighborhoodResult(productId, topThreeProductIds));
            }

            // 按照第一列productID从小到大排序
            neighborhoodResults.sort((r1, r2) -> r1.productId.compareTo(r2.productId));

            // 写入productNeighborhood.txt文件
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(productNeighborhoodFilePath))) {
                writer.write("productID;similarProductID1;similarProductID2;similarProductID3");
                writer.newLine();
                for (ProductNeighborhoodResult result : neighborhoodResults) {
                    writer.write(result.productId + ";" + result.similarProductIds.get(0) + ";"
                            + result.similarProductIds.get(1) + ";" + result.similarProductIds.get(2));
                    writer.newLine();
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // 计算Jaccard index
    private static double calculateJaccardIndex(List<String> set1, List<String> set2) {
        if (set1 == null || set2 == null) {
            return 0;
        }
        List<String> intersection = new ArrayList<>(set1);
        intersection.retainAll(set2);
        List<String> union = new ArrayList<>(set1);
        union.addAll(set2);
        union = new ArrayList<>(new HashSet<>(union));
        return (double) intersection.size() / union.size();
    }

    // 用于存储相似度结果的内部类
    private static class SimilarityResult {
        String productId;
        double similarity;

        public SimilarityResult(String productId, double similarity) {
            this.productId = productId;
            this.similarity = similarity;
        }
    }

    // 用于存储产品邻域结果的内部类
    private static class ProductNeighborhoodResult {
        String productId;
        List<String> similarProductIds;

        public ProductNeighborhoodResult(String productId, List<String> similarProductIds) {
            this.productId = productId;
            this.similarProductIds = similarProductIds;
        }
    }
}