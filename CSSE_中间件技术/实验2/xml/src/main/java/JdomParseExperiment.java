import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;

import java.io.File;
import java.io.FileOutputStream;
import java.util.List;

public class JdomParseExperiment {

    public static void main(String[] args) {
        // XML文件路径
        String xmlFilePath = "books.xml";

        try {
            // 1. 实例化SAXBuilder对象，用于解析XML
            SAXBuilder saxBuilder = new SAXBuilder();
            Document document = saxBuilder.build(new File(xmlFilePath));

            // 获取根节点 <books>
            Element rootElement = document.getRootElement();

            // ---------------------------------------------------------
            // 输出该 XML中描述的所有图书信息
            // ---------------------------------------------------------
            System.out.println("========== 原始图书信息 ==========");
            // JDOM 1.1 返回的是无泛型的 List，需要进行类型转换
            List bookList = rootElement.getChildren("book");
            for (int i = 0; i < bookList.size(); i++) {
                Element book = (Element) bookList.get(i);
                String id = book.getAttributeValue("id");
                String name = book.getChildText("name");
                String price = book.getChildText("price");
                System.out.println("BookID: " + id + " | Name: " + name + " | Price: " + price);
            }

            // ---------------------------------------------------------
            // 修改书名为《Lincon》的价格为30.5
            // ---------------------------------------------------------
            for (int i = 0; i < bookList.size(); i++) {
                Element book = (Element) bookList.get(i);
                if ("Lincon".equals(book.getChildText("name"))) {
                    // 找到Lincon，修改其price节点的内容
                    book.getChild("price").setText("30.5");
                    System.out.println("\n已成功将《Lincon》的价格修改为 30.5");
                    break;
                }
            }

            // ---------------------------------------------------------
            // 添加一本新书信息 (ID:5, name:中间件技术, price:39.0)
            // ---------------------------------------------------------
            Element newBook = new Element("book");
            newBook.setAttribute("id", "5"); // 设置属性

            Element newName = new Element("name").setText("中间件技术");
            Element newPrice = new Element("price").setText("39.0");

            newBook.addContent(newName);
            newBook.addContent(newPrice);

            // 将新书添加到根节点
            rootElement.addContent(newBook);
            System.out.println("已成功添加新书《中间件技术》\n");

            // ---------------------------------------------------------
            // 将修改后的文档输出并保存
            // ---------------------------------------------------------
            System.out.println("========== 更新后的图书信息 (XML结构) ==========");


            // 设置输出格式
            XMLOutputter xmlOutputter = new XMLOutputter(Format.getPrettyFormat());

            // 输出到控制台查看
            xmlOutputter.output(document, System.out);

            // 将修改后的内容保存文件
            xmlOutputter.output(document, new FileOutputStream("books_updated.xml"));
            System.out.println("\n更新后的数据已保存至 books_updated.xml 文件中。");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}