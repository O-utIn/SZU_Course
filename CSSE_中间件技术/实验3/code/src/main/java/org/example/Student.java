package org.example;

import java.lang.String;

public class Student {
    private String name;
    private int age;

    // 构造方法
    public Student(String name, int age) {
        this.name = name;
        this.age = age;
    }

    // Getter 和 Setter 方法
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    // toString 方法，将 name 和 age 连成一个字符串
    @Override
    public String toString() {
        return "Name: " + name + ", Age: " + age;
    }
}
