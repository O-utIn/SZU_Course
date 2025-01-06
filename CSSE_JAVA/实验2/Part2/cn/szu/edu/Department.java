package Part2.cn.szu.edu;

public class Department {
    public String name;
    public String chairman;

    public Department(String name, String chairman) {
        this.name = name;
        this.chairman = chairman;
    }

    public String toString() {
        return name + ",主任:" + chairman;
    }
}
