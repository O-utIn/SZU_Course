class Rectangle: #创建一个类
    def __init__(self,width,height): #设置宽和高两个属性
        self.width = width
        self.height = height

    def area(self): #方法area()用于计算矩形面积
        return self.width * self.height

#实例化对象，打印结果
jx = Rectangle(5,3)
s = jx.area()
print("面积:",s)
