class Calculator:
    def __init__(self,name):
        self.name = name
        self.__result = 0

    #加法
    def add(self,num):
        self.__result += num

    #减法
    def subtract(self,num):
        self.__result -= num

    #乘法
    def multiply(self,num):
        self.__result *= num

    #除法
    def divide(self,num):
        if num != 0:
            self.__result /= num
        else:
            print("错误！除数不能为0！")

    #获取计算结果
    def get_result(self):
        return self.__result

calculator = Calculator("普通计算器")
calculator.add(5)
calculator.subtract(2)
calculator.multiply(3)
calculator.divide(4)
print("计算得出的结果为:",calculator.get_result())
