class Country:
    continent = 'Asia'

    def __init__(self,name):
        self.name = name
        
    #用于返回国家所在的大洲
    def get_continent(self):
        return Country.continent

    #用于返回国家的名称
    def get_name(self):
        return self.name

country1 = Country("China")
print("所在的大洲是:",country1.get_continent())
print("该国家为:",country1.get_name())
