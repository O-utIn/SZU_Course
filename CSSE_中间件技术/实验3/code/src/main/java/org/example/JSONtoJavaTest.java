package org.example;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import java.lang.reflect.Type;

public class JSONtoJavaTest {

    // 将 Student 对象转换为 JSON 字符串
    public static String javaObjToJSONstr() {
        Student student = new Student("方依依", 19);
        Gson gson = new Gson();
        String jsonStr = gson.toJson(student); // 使用 Gson 将对象转换为 JSON 字符串
        System.out.println("Java Object to JSON String: " + jsonStr);
        return jsonStr;
    }

    // 将 Student 数组转换为 JSON 字符串
    public static String javaArrToJSONstr() {
        Student[] students = {
                new Student("同学1", 20),
                new Student("同学2", 21),
                new Student("同学3", 22)
        };
        Gson gson = new Gson();
        String jsonStr = gson.toJson(students); // 将数组转换为 JSON 字符串
        System.out.println("Java Array to JSON String: " + jsonStr);
        return jsonStr;
    }

    // 将 JSON 字符串转换为 Student 对象
    public static Student jsonStrTojavaObj(String jsonStr) {
        Gson gson = new Gson();
        Student student = gson.fromJson(jsonStr, Student.class); // 使用 Gson 将 JSON 字符串转换为 Student 对象
        System.out.println("JSON String to Java Object: " + student.toString());
        return student;
    }

    // 将 JSON 字符串转换为 Student 数组
    public static Student[] jsonStrToJavaArry(String jsonStr) {
        Gson gson = new Gson();
        Type studentListType = new TypeToken<Student[]>(){}.getType();
        Student[] students = gson.fromJson(jsonStr, studentListType); // 将 JSON 字符串转换为 Student 数组
        System.out.println("JSON String to Java Array:");
        for (Student student : students) {
            System.out.println(student.toString());
        }
        return students;
    }

    public static void main(String[] args) {
        // 测试：将 Java 对象转换为 JSON 字符串
        String studentJson = javaObjToJSONstr();

        // 测试：将 Java 数组转换为 JSON 字符串
        String studentsJson = javaArrToJSONstr();

        // 测试：将 JSON 字符串转换为 Java 对象
        String jsonStr = "{\"name\":\"方依依\",\"age\":17}";
        jsonStrTojavaObj(jsonStr);

        // 测试：将 JSON 字符串转换为 Java 数组
        String jsonArrStr = "[{\"name\":\"何立立\",\"age\":21}, {\"name\":\"赵多多\",\"age\":22}]";
        jsonStrToJavaArry(jsonArrStr);
    }
}
