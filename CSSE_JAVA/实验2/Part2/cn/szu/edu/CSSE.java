package Part2.cn.szu.edu;

import java.util.ArrayList;
import java.util.List;

public class CSSE {
    private List<Institute> institutes;
    private List<Department> departments;
    private List<Department> adminOffice;
    private List<Department> labCenter;
    private List<Department> journalEditorial;

    public CSSE() {
        this.institutes = new ArrayList<>();
        this.departments = new ArrayList<>();
        this.adminOffice = new ArrayList<>();
        this.labCenter = new ArrayList<>();
        this.journalEditorial = new ArrayList<>();
    }

    //添加研究所
    public void addIns(Institute ins) {
        institutes.add(ins);
    }

    //添加教学系
    public void addDept(Department dept) {
        departments.add(dept);
    }

    //添加行政办公室
    public void addAO(Department adminO) {
        adminOffice.add(adminO);
    }

    //添加实验中心
    public void addlabC(Department labC) {
        labCenter.add(labC);
    }

    //添加期刊编辑部
    public void addjournalE(Department jE) {
        journalEditorial.add(jE);
    }

    //获取所有研究所的名字及负责人
    public void getInstituteNames() {
        for (Institute ins : institutes) {
            System.out.println(ins);
        }
    }

    //获取所有教学系的名字及系主任
    public void getDepartmentNames() {
        for (Department dept : departments) {
            System.out.println(dept);
        }
    }

    //获取所有行政办公室的名字及负责人
    public void getAdminOfficeNames() {
        for (Department adminO : adminOffice) {
            System.out.println(adminO);
        }
    }

    //获取所有实验中心的名字及负责人
    public void getLabCenterNames() {
        for (Department labC : labCenter) {
            System.out.println(labC);
        }
    }

    //获取所有期刊编辑部的名字及负责人
    public void getJournalEditorialNames() {
        for (Department jE : journalEditorial) {
            System.out.println(jE);
        }
    }

    //研究所数量
    public int getInstituteNumber() {
        return institutes.size();
    }

    //教学系数量
    public int getDepartmentNumber() {
        return departments.size();
    }

    //行政办公室数量
    public int getAdminOfficeNumber() {
        return adminOffice.size();
    }

    //实验中心数量
    public int getLabCenterNumber() {
        return labCenter.size();
    }

    //期刊编辑部数量
    public int getJournalEditorialNumber() {
        return journalEditorial.size();
    }

    public static void main(String[] args) {
        CSSE csse = new CSSE();

        // 添加研究所
        csse.addIns(new Institute("高性能计算研究所", "陈国良"));
        csse.addIns(new Institute("大数据技术与应用研究所", "黄哲学"));
        csse.addIns(new Institute("未来媒体技术与计算研究所", "江健民"));
        csse.addIns(new Institute("网络与信息安全研究所", "李坚强"));
        csse.addIns(new Institute("计算机视觉研究所", "文振焜"));
        csse.addIns(new Institute("可视计算研究中心", "黄惠"));
        csse.addIns(new Institute("智能服务计算研究中心", "张良杰"));
        csse.addIns(new Institute("智能技术与系统集成研究所", "朱安民"));
        csse.addIns(new Institute("软件工程研究中心", "明仲"));

        // 添加教学系
        csse.addDept(new Department("计算机科学与技术系", "潘微科"));
        csse.addDept(new Department("软件工程系", "张良杰"));
        csse.addDept(new Department("人工智能系", "王熙照"));

        //添加行政办公室
        csse.addAO(new Department("党务工作","杨国洪"));
        csse.addAO(new Department("教学业务","胡沛"));
        csse.addAO(new Department("实验中心","林佳利"));
        csse.addAO(new Department("辅导员","黄晓聪"));
        csse.addAO(new Department("科研外事","何文锋"));
        csse.addAO(new Department("综合业务","刘晔"));

        //添加实验中心
        csse.addlabC(new Department("计算机实验教学示范中心","黄惠"));
        csse.addlabC(new Department("网络工程虚拟仿真实验教学中心","朱安民"));

        //添加期刊编辑部
        csse.addjournalE(new Department("期刊编辑部","王熙照"));

        // 输出所有研究所信息
        csse.getInstituteNames();

        // 输出所有教学系信息
        csse.getDepartmentNames();

        // 输出所有行政办公室信息
        csse.getAdminOfficeNames();

        // 输出所有实验中心信息
        csse.getLabCenterNames();

        // 输出所有期刊编辑部信息
        csse.getJournalEditorialNames();

        // 输出研究所数量
        System.out.println("研究所数量:" + csse.getInstituteNumber());

        // 输出教学系数量
        System.out.println("教学系数量:" + csse.getDepartmentNumber());

        // 输出行政办公室数量
        System.out.println("行政办公室数量:" + csse.getAdminOfficeNumber());

        // 输出实验中心数量
        System.out.println("实验中心数量:" + csse.getLabCenterNumber());

        // 输出期刊编辑部数量
        System.out.println("期刊编辑部数量:" + csse.getJournalEditorialNumber());
    }

}
