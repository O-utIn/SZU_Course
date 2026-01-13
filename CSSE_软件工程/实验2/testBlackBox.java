
/**
 * Person类包含了纳税人的子女个数、继续教育情况、住房贷款情况、租房信息、赡养老人情况、兄弟姐妹个数
 */
class Person {
    private int children_Num; //子女个数——每个子女每月扣除1000元
    private int is_KeepStu;  //是否继续教育（0：未继续教育  1：接受继续教育且当年未获取到证书——每月扣除400元  2：接受继续教育且当年获取到证书——当年扣除定额3600元）
    private boolean HouseLoans;  //是否住房贷款（false：没有  true：有）——如果有，每个月扣除1000元
    private int is_RentHouse;   // 是否住房（0：没有住房
    //         1：在直辖市、省会（首府）城市租房——每月扣除1500元
    //         2：在除第一项所列城市以外，市辖区户籍人口超过100万的城市租房——每月扣除1100元
    //         3：市辖区户籍人口不超过100万的城市租房——每月扣除800元。）
    private boolean SupportingElderly;   //是否赡养老人（false：没有  true：有）——赡养老人每个月扣除2000元/兄弟姐妹个数+1
    private int sibling_Num; //兄弟姐妹个数

    //Constructor
    public Person(int children_Num, int is_KeepStu, boolean is_HouseLoans, int is_RentHouse, boolean is_SupportingElderly, int sibling_Num) {
        this.children_Num = children_Num;
        this.is_KeepStu = is_KeepStu;
        this.HouseLoans = is_HouseLoans;
        this.is_RentHouse = is_RentHouse;
        this.SupportingElderly = is_SupportingElderly;
        this.sibling_Num = sibling_Num;
    }

    //Getter
    public int getChildren_Num() {
        return children_Num;
    }

    public int getIs_KeepStu() {
        return is_KeepStu;
    }

    public boolean is_HouseLoans() {
        return HouseLoans;
    }

    public int getIs_RentHouse() {
        return is_RentHouse;
    }

    public boolean is_SupportingElderly() {
        return SupportingElderly;
    }

    public int getSibling_Num() {
        return sibling_Num;
    }
}

//个人所得税计算器
class incomeTaxCalculator {
    /**
     * 计算个人所得税
     *
     * @param person 纳税人
     * @param salary 工资(已扣除三险一金)
     * @return
     */
    public static int cal(Person person, int salary) {
        //安全性检查
        if (person == null) {
            throw new NullPointerException("person为空");
        }
        if (salary < 0) {
            throw new IllegalArgumentException();
        }

        //应纳税所得额=月度收入-5000元（免征额）-专项扣除（三险一金等）-专项附加扣除-依法确定的其他扣除
        int res = salary - 5000;

        //专项附加扣除
        //子女教育:每个子女每月1000元
        if (person.getChildren_Num() > 0) {
            res -= person.getChildren_Num() * 1000;
        }

        //继续教育:在学历（学位）教育期间每月400元,在取得相关证书的当年，按照3600元定额扣除
        if (person.getIs_KeepStu() == 1) {
            res -= 400;
        } else if (person.getIs_KeepStu() == 2) {
            res -= 3600;
        }

        //住房贷款:在实际发生贷款利息的年度按每月1000元扣除
        if (person.is_HouseLoans()) {
            res -= 1000;
        }

        //住房租金: 1、直辖市、省会（首府）城市:每月1500元
        //         2、市辖区户籍人口超过100万的城市：每月1100元
        //         3、市辖区户籍人口不超过100万的城市：每月800元
        if (person.getIs_RentHouse() == 1) {
            res -= 1500;
        } else if (person.getIs_RentHouse() == 2) {
            res -= 1100;
        } else if (person.getIs_RentHouse() == 3) {
            res -= 800;
        }

        //赡养老人: 每个月扣除2000元/兄弟姐妹个数+1
        if (person.is_SupportingElderly()) {
            res -= 2000 / (person.getSibling_Num() + 1);
        }

        //计算个人所得税：个人所得税=应纳税所得额*税率
        double tax = res;
        //根据阶梯税率计算结果
        if (res < 0) {
            tax = 0;
        } else if (res < 3000) {         //0~3000元 税率3%
            tax = res * 0.03;
        } else if (res < 12000) {    //3000~12000元 税率10%
            tax = 90 + (res - 3000) * 0.1;
        } else if (res < 25000) {    //12000~25000元 税率20%
            tax = 2290 + (res - 12000) * 0.2;
        } else if (res < 35000) {    //25000~35000元 税率25%
            tax = 4890 + (res - 25000) * 0.25;
        } else if (res < 55000) {    //35000~55000元 税率30%
            tax = 7390 + (res - 35000) * 0.3;
        } else if (res < 80000) {    //55000~80000元 税率35%
            tax = 13390 + (res - 55000) * 0.35;
        } else {                      //大于80000部分税率40%
            tax = 22140 + (res - 80000) * 0.45;
        }

        return (int) tax; //返回结果取整
    }
}


public class testBlackBox {
    public static void main(String[] args) {

        // 用例 1: 普通收入无任何扣除项
        runTestCase("1. 普通收入无任何扣除项", 
                new Person(0, 0, false, 0, false, 0), 5000, 0);

        // 用例 2: 边界条件测试起征点
        runTestCase("2. 边界条件测试起征点", 
                new Person(0, 0, false, 0, false, 0), 5000, 0);

        // 用例 3: 起征点以上少量收入
        runTestCase("3. 起征点以上少量收入", 
                new Person(0, 0, false, 0, false, 0), 6000, 30);

        // 用例 4: 多项扣除测试
        runTestCase("4. 多项扣除测试", 
                new Person(2, 1, true, 1, true, 1), 50000, 8620);

        // 用例 5: 高收入测试
        runTestCase("5. 高收入测试", 
                new Person(0, 0, false, 0, false, 0), 100000, 28890);

        // 用例 6: 负收入测试
        try {
            Person p = new Person(0, 0, false, 0, false, 0);
            incomeTaxCalculator.cal(p, -1000);
            System.out.println("6. 负收入测试: Failed - No exception thrown");
        } catch (IllegalArgumentException e) {
            System.out.println("6. 负收入测试: Passed - Caught IllegalArgumentException");
        } catch (Exception e) {
            System.out.println("6. 负收入测试: Failed - Wrong exception: " + e);
        }

        // 用例 7: 非法参数测试
        try {
            incomeTaxCalculator.cal(null, 30000);
            System.out.println("7. 非法参数测试: Failed - No exception thrown");
        } catch (NullPointerException e) {
            System.out.println("7. 非法参数测试: Passed - Caught NullPointerException");
        } catch (Exception e) {
            System.out.println("7. 非法参数测试: Failed - Wrong exception: " + e);
        }
        
    }

    /**
     * 辅助方法：用于运行正常计算的测试用例并比较结果
     */
    public static void runTestCase(String testName, Person person, int salary, int expectedResult) {
        try {
            int actualResult = incomeTaxCalculator.cal(person, salary);
            if (actualResult == expectedResult) {
                System.out.println(testName + ": Passed - Tax Calculated: " + actualResult);
            } else {
                System.out.println(testName + ": Failed - Expected " + expectedResult + " but got " + actualResult);
            }
        } catch (Exception e) {
            System.out.println(testName + ": Failed - Exception thrown: " + e);
        }
    }
}