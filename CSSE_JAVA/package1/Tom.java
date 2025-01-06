package package1;

public class Tom{
	 int leg;
	 String head;
	 void cry(String s){
	 System.out.println(s);
	 }
	}
	class Example{
	 public static void main (String args[])
	 { Tom cat;
	 cat=new Tom();
	 cat.leg=4;
	 cat.head="猫头";
	 System.out.println("腿:"+cat.leg+"条");
	 System.out.println("头:"+cat.head);
	 cat.cry("我今天要和Jerry拼了");
	 }
}