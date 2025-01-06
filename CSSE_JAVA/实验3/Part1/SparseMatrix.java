package Part1;

import java.text.DecimalFormat;
import java.util.HashMap;

public class SparseMatrix {
    HashMap<Integer, HashMap<Integer, Double>> matrix;

    public SparseMatrix() {
        matrix = new HashMap<>();
    }

    // 向矩阵中填值
    public void setValue(int row, int col, double value) {
        if (!matrix.containsKey(row)) {
            matrix.put(row, new HashMap<>());
        }
        matrix.get(row).put(col, value);
    }

    // 获取矩阵中的元素
    public double getValue(int row, int col) {
        double value = matrix.getOrDefault(row, new HashMap<>()).getOrDefault(col, 0.0);
        return Double.parseDouble(String.valueOf(value));
    }

    // 输出稀疏矩阵
    public void printMatrix() {
        for (Integer row : matrix.keySet()) {
            HashMap<Integer, Double> rowMap = matrix.get(row);
            for (Integer col : rowMap.keySet()) {
                double value = rowMap.get(col);
                if (value!= 0) {
                    DecimalFormat df = new DecimalFormat("#.##");
                    System.out.println("行号：" + row + " 列号：" + col + " 值：" + df.format(value));
                }
            }
        }
    }

    //两个矩阵相乘
    public SparseMatrix multiply(SparseMatrix other) {
        SparseMatrix result = new SparseMatrix();
        int rows1 = 0;
        for (Integer row : matrix.keySet()) {
            rows1 = Math.max(rows1, row);
        }
        rows1++;
        int cols1 = 0;
        for (HashMap<Integer, Double> rowMap : matrix.values()) {
            for (Integer col : rowMap.keySet()) {
                cols1 = Math.max(cols1, col);
            }
        }
        cols1++;
        int cols2 = 0;
        for (Integer row : other.matrix.keySet()) {
            cols2 = Math.max(cols2, row);
        }
        cols2++;
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                double sum = 0;
                for (int k = 0; k < cols1; k++) {
                    sum += getValue(i, k) * other.getValue(k, j);
                }
                if (sum!= 0) {
                    result.setValue(i, j, sum);
                }
            }
        }
        return result;
    }

    //两个矩阵相加
    public SparseMatrix add(SparseMatrix other) {
        SparseMatrix result = new SparseMatrix();
        int rows1 = 0;
        for (Integer row : matrix.keySet()) {
            rows1 = Math.max(rows1, row);
        }
        rows1++;
        int cols1 = 0;
        for (HashMap<Integer, Double> rowMap : matrix.values()) {
            for (Integer col : rowMap.keySet()) {
                cols1 = Math.max(cols1, col);
            }
        }
        cols1++;
        int rows2 = 0;
        for (Integer row : other.matrix.keySet()) {
            rows2 = Math.max(rows2, row);
        }
        rows2++;
        int cols2 = 0;
        for (HashMap<Integer, Double> rowMap : other.matrix.values()) {
            for (Integer col : rowMap.keySet()) {
                cols2 = Math.max(cols2, col);
            }
        }
        cols2++;
        if (rows1!= rows2 || cols1!= cols2) {
            System.out.println("矩阵维度不匹配，无法相加。");
            return null;
        }
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                double sum = getValue(i, j) + other.getValue(i, j);
                if (sum!= 0) {
                    result.setValue(i, j, sum);
                }
            }
        }
        return result;
    }



    public static void main(String[] args) {
        SparseMatrix matrix1 = new SparseMatrix();
        matrix1.setValue(0, 0, 1.2);
        matrix1.setValue(1, 2, 3.1);
        matrix1.setValue(2, 3, 2.2);
        System.out.println("Matrix 1(3*4): ");
        matrix1.printMatrix();

        SparseMatrix matrix2 = new SparseMatrix();
        matrix2.setValue(0, 1, 1.1);
        matrix2.setValue(0, 2, 1.5);
        matrix2.setValue(1, 2, 3.2);
        matrix2.setValue(2, 1, 1.3);
        matrix2.setValue(2, 4, -3.2);
        matrix2.setValue(3, 0, -1.0);
        matrix2.setValue(3, 1, 6.2);
        System.out.println("Matrix 2(4*5): ");
        matrix2.printMatrix();

        SparseMatrix matrix3 = new SparseMatrix();
        matrix3=matrix1.multiply(matrix2);
        System.out.println("Matrix 3: ");
        matrix3.printMatrix();

        SparseMatrix matrix4 = new SparseMatrix();
        matrix4.setValue(0,0,1.2);
        matrix4.setValue(0,1,-3);
        matrix4.setValue(0,2,-5.3);
        matrix4.setValue(1,1,1);
        matrix4.setValue(1,2,0.1);
        matrix4.setValue(1,3,-0.4);
        matrix4.setValue(2,0,2);
        matrix4.setValue(2,1,2);
        matrix4.setValue(2,2,1);
        matrix4.setValue(2,3,0.2);
        System.out.println("Matrix 4(3*4): ");
        matrix4.printMatrix();

        SparseMatrix matrix5 = new SparseMatrix();
        matrix5=matrix1.add(matrix4);
        System.out.println("Matrix 5(3*4): ");
        matrix5.printMatrix();

    }
}