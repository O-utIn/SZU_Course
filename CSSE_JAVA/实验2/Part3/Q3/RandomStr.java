package Part3.Q3;
import java.util.Random;

public class RandomStr {
    //生成随机混杂字符串
    private static String generateStr(Random random) {
        StringBuilder newS = new StringBuilder();
        for (int i = 0; i < 10; i++) {
            //随机选择一个类型（大写字母、小写字母、数字、其他字符）
            int choice = random.nextInt(4);

            if (choice == 0)
            {
                // 生成大写字母并添加到字符串中
                newS.append((char) (random.nextInt(26) + 'A'));
            }
            else if (choice == 1)
            {
                // 生成小写字母并添加到字符串中
                newS.append((char) (random.nextInt(26) + 'a'));
            }
            else if (choice == 2)
            {
                // 生成数字并添加到字符串中
                newS.append(random.nextInt(10));
            }
            else
            {
                // 生成其他特殊字符并添加到字符串中
                char specialChar = (char) (random.nextInt(15) + 33);
                newS.append(specialChar);
            }
        }
        return newS.toString();
    }

    //解析字符串
    private static void parseAndPrint(String str) {
        StringBuilder upperCase = new StringBuilder();
        StringBuilder lowerCase = new StringBuilder();
        StringBuilder digits = new StringBuilder();
        StringBuilder others = new StringBuilder();

        for (char c : str.toCharArray()) {
            if (Character.isUpperCase(c))
            {
                // 如果是大写字母，添加到大写字母字符串中
                upperCase.append(c);
            }
            else if (Character.isLowerCase(c))
            {
                // 如果是小写字母，添加到小写字母字符串中
                lowerCase.append(c);
            }
            else if (Character.isDigit(c))
            {
                // 如果是数字，添加到数字字符串中
                digits.append(c);
            }
            else
            {
                // 如果是其他字符，添加到其他字符字符串中
                others.append(c);
            }
        }

        System.out.println("大写字母：" + upperCase);
        System.out.println("小写字母：" + lowerCase);
        System.out.println("数字：" + digits);
        System.out.println("其他字符：" + others);
    }

    public static void main(String[] args) {
        // 创建一个随机数生成器对象
        Random random = new Random();
        for (int i = 0; i < 5; i++) {
            // 生成一个随机混杂的字符串
            String randomString = generateStr(random);
            System.out.println("原始字符串：" + randomString);
            // 解析并输出这个字符串的不同类型字符
            parseAndPrint(randomString);
            System.out.println();
        }
    }
}

