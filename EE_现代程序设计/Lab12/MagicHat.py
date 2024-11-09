import time

class MagicHat:
    def __init__(self):
        self._color = "black"

    #获取帽子颜色
    def get_color(self):
        print("    *\n  * * *\n* * * * *")
        return self._color

    #设置帽子颜色
    def set_color(self,value):
        print("帽子颜色变化中...")
        time.sleep(1)
        self._color = value
        print(f"帽子颜色变成了{self._color}")

    #实现对color属性的设置和获取
    color = property(get_color,set_color)

hat = MagicHat()
print(hat.color)
hat.color = "red"
print(hat.color)
