class Vehicle():
    def start(self):
        print("车辆启动")

class Car(Vehicle):
    def drive(self):
        print("正在驾驶\n")

class Motorcycle(Vehicle):
    def ride(self):
        print("正在骑行\n")

BenZ = Car()
Harley = Motorcycle()

BenZ.start()
BenZ.drive()

Harley.start()
Harley.ride()
