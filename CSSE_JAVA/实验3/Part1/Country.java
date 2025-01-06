package Part1;

public class Country implements Comparable<Country> {
    private String name;
    private double GDP2023;
    private int Olympics2024;

    public Country(String name, double GDP2023, int Olympics2024) {
        this.name = name;
        this.GDP2023 = GDP2023;
        this.Olympics2024 = Olympics2024;
    }

    public String getName() {
        return name;
    }

    public double getGDP2023() {
        return GDP2023;
    }

    public int getOlympics2024() {
        return Olympics2024;
    }

    @Override
    public int compareTo(Country other) {
        return Integer.compare(other.Olympics2024, this.Olympics2024);
    }

    @Override
    public String toString() {
        return "Country name='" + name + '\'' +
                ", GDP2023（亿元）=" + GDP2023 +
                ", Olympics2024=" + Olympics2024;
    }
}
