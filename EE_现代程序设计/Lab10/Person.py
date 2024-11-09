class Person: #创建一个Person类，定义name和age两个属性
    def __init__(self,name,age):
        self.name = name
        self.age = age

#实例化对象并打印
student1 = Person("小荔",19)
print("姓名:",student1.name)
print("年龄:",student1.age)
