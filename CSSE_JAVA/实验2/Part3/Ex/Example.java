package Part3.Ex;

public class Example {
    public static void main(String[] args) {
        Chinese C=new Chinese("小明");
        System.out.println(C.sayHello());
        Italian I=new Italian("Kery");
        System.out.println(I.sayHello());
        Spaniard S=new Spaniard("Mike");
        System.out.println(S.sayHello());
    }
}
