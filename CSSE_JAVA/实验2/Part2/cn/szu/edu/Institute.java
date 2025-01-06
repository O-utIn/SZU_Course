package Part2.cn.szu.edu;

public class Institute {
    public String name;
    public String director;

    public Institute(String name, String director) {
        this.name = name;
        this.director = director;
    }

    public String toString() {
        return name + ",负责人:" + director;
    }

}
