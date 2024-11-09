class Animal():
    def make_sound(self):
        print("click")

class Swimmer():
    def swim(self):
        print("stroke")

class Dolphin(Animal,Swimmer):
    pass

dolphin = Dolphin()
dolphin.make_sound()
dolphin.swim()
