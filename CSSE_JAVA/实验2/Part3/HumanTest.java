package Part3;

public class HumanTest {
    public Human []human;

    public HumanTest(){
        human = new Human[3];
        human[0]= new Chinese("老王");
        human[1]= new Spaniard("Bob");
        human[2]= new Italian("Tony");
    }

    public static void main(String[] args) {
        HumanTest ht = new HumanTest();
        for(int i=0;i<3;i++)
        {
            System.out.println(ht.human[i].sayHello());
        }
    }
}
