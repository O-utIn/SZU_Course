package Part1;

import java.util.Comparator;

public class GDPComparator implements Comparator<Country> {
    @Override
    public int compare(Country c1, Country c2) {
        return Double.compare(c2.getGDP2023(), c1.getGDP2023());
    }
}