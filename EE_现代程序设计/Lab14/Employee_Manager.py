class Employee:
    #存储员工信息
    employee_list = []

    #初始化员工属性
    def __init__(self, id, name, position, salary):
        self.id = id
        self.name = name
        self.position = position
        self.salary = salary
        Employee.employee_list.append(self)

    #创建一个新员工的实例
    def add_employee(self, id, name, position, salary):
        new_employee = Employee(id, name, position, salary)

    #更新现有员工的信息
    def edit_employee(self, id, name, position, salary):
        for employee in Employee.employee_list:
            if employee.id == id:
                employee.name = name
                employee.position = position
                employee.salary = salary

    #删除员工
    def delete_employee(self, id):
        for employee in Employee.employee_list:
            if employee.id == id:
                Employee.employee_list.remove(employee)

    #用于展示员工的基本信息
    def display_info(self):
        print(f"ID: {self.id}, Name: {self.name}, Position: {self.position}, Salary: {self.salary}")


class SystemSettings:
    #存储员工薪资信息的字典，键为职位，值为薪资
    def __init__(self):
        self.salary_table = {}

    #设置一个员工的薪资
    def set_salary(self, position, salary):
        self.salary_table[position] = salary


class EmployeeManager:
    #初始化经理属性
    def __init__(self, id, name):
        self.id = id
        self.name = name
        self.subordinates = [] #存储经理下属信息
        
    #将一个员工添加到另一个经理的管理下
    def add_subordinate(self, employee_id):
        self.subordinates.append(employee_id)

    #从经理的管理列表中移除一个员工
    def remove_subordinate(self, employee_id):
        if employee_id in self.subordinates:
            self.subordinates.remove(employee_id)


class Manager:
    #存储经理信息
    manager_list = []

    #初始化经理的属性
    def __init__(self, id, name):
        self.id = id
        self.name = name
        Manager.manager_list.append(self)

    #创建一个新经理的实例
    def add_manager(self, id, name):
        new_manager = Manager(id, name)

    #更新现有经理的信息
    def edit_manager(self, id, name):
        for manager in Manager.manager_list:
            if manager.id == id:
                manager.name = name

    #删除经理
    def delete_manager(self, id):
        for manager in Manager.manager_list:
            if manager.id == id:
                Manager.manager_list.remove(manager)

    #用于展示经理的基本信息
    def display_info(self):
        print(f"ID: {self.id}, Name: {self.name}")

class DataImporter:
    # 初始化文件路径
    def __init__(self, file_path):
        self.file_path = file_path

    # 从指定的文件中导入员工数据
    def import_data(self):
        # 实现导入数据的具体逻辑
        print(f"Importing data from {self.file_path}")


class DataExporter:
    # 初始化文件路径
    def __init__(self, file_path):
        self.file_path = file_path

    # 将员工数据导出到指定的文件中
    def export_data(self):
        # 实现导出数据的具体逻辑
        print(f"Exporting data to {self.file_path}")


# 示例
system = SystemSettings()
system.set_salary("Manager", 50000)
system.set_salary("Assistant", 30000)
system.set_salary("Clerk", 25000)

employee1 = Employee(1, "John", "Manager", 50000)
employee2 = Employee(2, "Alice", "Assistant", 30000)
employee3 = Employee(3, "Bob", "Clerk", 25000)

# 显示所有员工的信息
print("\n显示所有员工的信息：")
for employee in Employee.employee_list:
    employee.display_info()

# 显示特定员工的信息
print("\n显示特定员工的信息：")
employee2.display_info()

# 更新员工信息
employee2.edit_employee(2, "Alice", "Senior Assistant", 35000)
print("\n更新后的员工信息：")
for employee in Employee.employee_list:
    employee.display_info()

# 添加新员工
employee2.add_employee(4, "Eva", "Clerk", 28000)
print("\n添加新员工后的信息：")
for employee in Employee.employee_list:
    employee.display_info()

# 删除员工
employee2.delete_employee(3)
print("\n删除员工后的信息：")
for employee in Employee.employee_list:
    employee.display_info()
    
# 实例化经理信息
manager1 = Manager(1, "Michael")
manager2 = Manager(2, "Sarah")

# 显示所有经理的信息
print("\n所有经理的信息：")
for manager in Manager.manager_list:
    manager.display_info()

# 更新经理信息
manager2.edit_manager(2, "Sarah Smith")
print("\n更新后的经理信息：")
for manager in Manager.manager_list:
    manager.display_info()

# 添加新经理
manager3 = Manager(3, "Tom")
print("\n添加新经理后的信息：")
for manager in Manager.manager_list:
    manager.display_info()

# 删除经理
manager2.delete_manager(2)
print("\n删除经理后的信息：")
for manager in Manager.manager_list:
    manager.display_info()

# 实例化员工信息
employee1 = Employee(1, "John", "Manager", 50000)
employee2 = Employee(2, "Alice", "Assistant", 30000)
employee3 = Employee(3, "Bob", "Clerk", 25000)

# 实例化员工经理关系
employee1_manager = EmployeeManager(1, "Michael")
employee2_manager = EmployeeManager(2, "Sarah")
employee3_manager = EmployeeManager(1, "Michael")

# 添加下属
employee2_manager.add_subordinate(2)
employee2_manager.add_subordinate(3)

# 显示经理的下属
print("\n" + f"{manager1.name}的下属：{employee1.name}")
print("\n" + f"{manager2.name}的下属：{employee2.name}, {employee3.name}")

# 移除下属
employee2_manager.remove_subordinate(3)

# 显示经理的下属
print(f"\n{manager2.name}的下属：{employee2.name}\n")


importer = DataImporter(None)
importer.import_data()

exporter = DataExporter(None)
exporter.export_data()
