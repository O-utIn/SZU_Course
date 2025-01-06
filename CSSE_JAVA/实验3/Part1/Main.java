package Part1;
import java.util.Comparator;
import java.util.TreeSet;

public class Main {
    public static void main(String[] args) {
        TreeSet<Country> countries = new TreeSet<>(new Comparator<Country>() {
            @Override
            public int compare(Country o1, Country o2) {
                return o2.compareTo(o1);
            }
        });

        countries.add(new Country("美国",273600,126));
        countries.add(new Country("中华人民共和国",177900,91));
        countries.add(new Country("德国",273600,33));
        countries.add(new Country("日本",42100,45));
        countries.add(new Country("英国",33400,65));
        countries.add(new Country("印度",35500,6));
        countries.add(new Country("法国",30300,64));
        countries.add(new Country("意大利",22500,40));
        countries.add(new Country("加拿大",21400,27));
        countries.add(new Country("韩国",17100,32));
        countries.add(new Country("以色列",5099.01,7));
        countries.add(new Country("俄罗斯",20200,0));

        // 按照 Olympics2024 从大到小排序
        TreeSet<Country> olympicsCompare = new TreeSet<>(new OlympicsComparator());
        for (Country country : countries) {
            olympicsCompare.add(country);
        }
        System.out.println("按照 Olympics2024 从大到小排序：");
        for (Country country : olympicsCompare) {
            System.out.println(country);
        }

        System.out.println();

        // 按照 GDP2023 从大到小排序
        TreeSet<Country> GDPCompare = new TreeSet<>(new GDPComparator());
        for (Country country : countries) {
            GDPCompare.add(country);
        }
        System.out.println("按照 GDP2023 从大到小排序：");
        for (Country country : GDPCompare) {
            System.out.println(country);
        }
    }
}