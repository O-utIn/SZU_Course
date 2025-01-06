package Part1;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class CSRanking {
    public Map<String,String> data;

    public CSRanking() {
        this.data = new HashMap<>();

        data.put("Machine Learning & Data Mining",
                "会议名称：ICML网址：dblp.org/db/conf/icml/index.html\n" +
                "会议名称：KDD网址：dblp.org/db/conf/kdd/index.html\n" +
                "会议名称：NeurIPS网址：dblp.org/db/conf/nips/index.html\n");
        data.put("Databases",
                "会议名称：VLDB网址：dblp.org/db/conf/vldb/index.html\n" +
                "会议名称：SIGMOD网址：dblp.org/db/conf/sigmod/index.html\n" +
                "会议名称：ICDE网址：dblp.org/db/conf/icde/index.html\n" +
                "会议名称：PODS网址：dblp.org/db/conf/pods/index.html\n");
        data.put("Software Engineering",
                "会议名称：FSE网址：dblp.org/db/conf/fse/index.html\n" +
                "会议名称：ICSE网址：dblp.org/db/conf/icse/index.html\n" +
                "会议名称：ASE网址：dblp.org/db/conf/kbse/index.html\n" +
                "会议名称：ISSTA网址：dblp.org/db/conf/issta/index.html\n");
        data.put("The Web & Information Retrieval",
                "会议名称：SIGIR网址：dblp.org/db/conf/sigir/index.html\n" +
                "会议名称：WWW网址：dblp.org/db/conf/www/index.html\n");
        data.put("Computer Graphics",
                "会议名称：SIGGRAPH网址：dblp.org/db/conf/siggraph/index.html\n" +
                "会议名称：SIGGRAPH ASIA网址：dblp.org/db/conf/siggrapha/index.html\n");
    }

    public String toString(String input) {
        if (data.containsKey(input)) {
            return data.get(input);
        } else {
            return "未找到指定研究方向的会议信息。";
        }
    }

    public static void main(String[] args) {
        CSRanking R = new CSRanking();
        Scanner sc = new Scanner(System.in);
        for(int i=0;i<5;i++) {
            String input = sc.nextLine();
            System.out.println(R.toString(input));
        }
    }

}
