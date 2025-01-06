package Part1;

import java.util.Comparator;

public class OlympicsComparator implements Comparator<Country> {
    @Override
    public int compare(Country c1, Country c2) {
        return Integer.compare(c2.getOlympics2024(), c1.getOlympics2024());
    }
}
