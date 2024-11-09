class Cart:
    def __init__(self):
        self.items = []

    #添加实例方法，用于向购物车中添加商品
    def add_item(self, name, price):
        if price < 0:
            print("请输入合法的价格信息！")
        else:
            self.items.append((name, price))
            
    #添加实例方法，用于从购物车移除指定名称的商品
    def remove_item(self, name):
        for item in self.items:
            if item[0] == name:
                self.items.remove(item)
                return
        print("未找到该商品信息！")

    #添加实例方法，用于计算购物车中商品的总价格
    def calculate_total(self):
        total = 0
        for item in self.items:
            total += item[1]
        return total

    #类方法，用于返回折扣后的价格
    @classmethod
    def get_discounted_price(cls, price, discount):
        return price * (1 - discount) #折扣后价格 = 原始价格 * (1 - 折扣率)

    #静态方法，用于将价格转换为其他货币的金额
    @staticmethod
    def convert_currency(price, exchange_rate):
        return price * exchange_rate #转换后金额 = 原始价格 * 汇率



cart = Cart()
cart.add_item("Shirt", 20)
cart.add_item("Pants", 30)
cart.add_item("Shoes", 50)

print(cart.calculate_total())  #期望输出 100

cart.remove_item("Pants")
print(cart.calculate_total())  #期望输出 70

discounted_price = Cart.get_discounted_price(100, 0.2)
print(discounted_price)  #期望输出 80.0

converted_price = Cart.convert_currency(100, 0.8)
print(converted_price)  #期望输出 80.0

cart.add_item("Hat", -80) #期望输出错误信息提示
cart.remove_item("Doors") #期望输出错误信息提示
