package Part3.Q3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ReadStr {
    //统计给定文本中每个单词的出现次数
    private static Map<String, Integer> wordCount(String content) {
        //将文本转换为小写形式
        String lowerCase = content.toLowerCase();
        //使用正则表达式将文本分割成单词数组
        String[] words = lowerCase.split("[^a-zA-Z]+");
        //创建一个哈希表存储单词及其出现次数
        Map<String, Integer> freqMap = new HashMap<>();
        //遍历单词数组，统计每个单词的出现次数
        for(String word : words)
        {
            if(!word.isEmpty())
            {
                freqMap.put(word, freqMap.getOrDefault(word, 0) + 1);
            }
        }
        return freqMap;
    }

    public static void main(String[] args) {
        //定义要分析的文本内容
        String text = "The Computer Science and Technology discipline at Shenzhen University was founded with support " +
                "from Tsinghua University in 1983 when the Shenzhen University was just established. College of " +
                "Computer Science and Software Engineering was formally established in 2008, which was headed by " +
                "Guoliang Chen, the Academician of the Chinese Academy of Sciences, as its founding Dean. Over the " +
                "years' development, CSSE has now led by Professor Hui Huang and become a prominent college in " +
                "education and research nationally and globally.\n" +
                "The platforms include two national-level research platforms (National Engineering Laboratory for " +
                "the Big Data System Computing Technology, MOE-GD Collaborative Innovation Center for GD-HK Modern " +
                "Information Service), three national-level teaching platforms (Computer Experimental Teaching " +
                "Center, Virtual Simulation Experimental Center for Network Engineering, and Tencent Cloud AI " +
                "College), the Guangdong Laboratory of Artificial Intelligence and Digital Economy (Shenzhen), ten " +
                "other provincial-level and ten municipal-level platforms.";

        //统计文本中单词出现的次数，并存储在一个哈希表中
        Map<String, Integer> wordFreqMap = wordCount(text);

        //将哈希表转换为列表，以便进行排序操作
        List<Map.Entry<String, Integer>> wordFreqList = new ArrayList<>(wordFreqMap.entrySet());

        //对列表进行排序，先按出现次数降序，若次数相同则按字母顺序
        Collections.sort(wordFreqList, new Comparator<Map.Entry<String, Integer>>() {
            @Override
            public int compare(Map.Entry<String, Integer> o1, Map.Entry<String, Integer> o2) {
                int freqComparison = o2.getValue().compareTo(o1.getValue());
                if (freqComparison!= 0)
                {
                    return freqComparison;
                }
                else
                {
                    return o1.getKey().compareTo(o2.getKey());
                }
            }
        });

        //输出出现次数最多的前 10 个单词
        for (int i = 0; i < Math.min(10, wordFreqList.size()); i++) {
            Map.Entry<String, Integer> entry = wordFreqList.get(i);
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }
}