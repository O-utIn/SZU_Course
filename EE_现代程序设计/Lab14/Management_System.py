import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
import tkinter.messagebox as messagebox


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
        return f"ID: {self.id}, 姓名: {self.name}, 职位: {self.position}, 薪资: {self.salary}"



class Manager:
    #存储经理信息
    manager_list = []

    #初始化经理的属性
    def __init__(self, id, name, position, salary):
        self.id = id
        self.name = name
        self.position = position
        self.salary = salary
        self.subordinates = []
        Manager.manager_list.append(self)

    #创建一个新经理的实例
    def add_manager(self, id, name, position, salary):
        new_manager = Manager(id, name, position, salary)

    #更新现有经理的信息
    def edit_manager(self, id, name, position, salary):
        for manager in Manager.manager_list:
            if manager.id == id:
                manager.name = name
                manager.position = position
                manager.salary = salary

    #删除经理
    def delete_manager(self, id):
        for manager in Manager.manager_list:
            if manager.id == id:
                Manager.manager_list.remove(manager)

    #用于展示经理的基本信息
    def display_info(self):
        return f"ID: {self.id}, 姓名: {self.name}, 职位: {self.position}, 薪资: {self.salary}"

    #将一个员工添加到经理的管理下
    def add_subordinate(self, employee_id):
        self.subordinates.append(employee_id)

    #从经理的管理列表中移除一个员工
    def remove_subordinate(self, employee_id):
        if employee_id in self.subordinates:
            self.subordinates.remove(employee_id)

    def delete_subordinate(self, subordinate_name):
        for employee in Employee.employee_list:
            if employee.name == subordinate_name:
                self.subordinates.remove(employee.id)
                return f"员工 {subordinate_name} 已成功从经理 {self.name} 的管理下移除"
        return f"未找到名为 {subordinate_name} 的员工"



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



class EmployeeManagerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("公司员工管理系统")#设置用户界面标题

        #添加人员（员工/经理）功能
        #设置添加人员下拉框及选项
        self.position_var = tk.StringVar()
        self.position_var.set("员工")

        self.position_menu = ttk.Combobox(self.root, textvariable=self.position_var, values=["员工", "经理"])
        self.position_menu.pack(pady=5)

        self.name_label = tk.Label(self.root, text="姓名:")
        self.name_label.pack(pady=5)
        self.name_entry = tk.Entry(self.root)
        self.name_entry.pack(pady=5)

        self.salary_label = tk.Label(self.root, text="薪资:")
        self.salary_label.pack(pady=5)
        self.salary_entry = tk.Entry(self.root)
        self.salary_entry.pack(pady=5)

        self.add_button = tk.Button(self.root, text="添加人员", command=self.add_employee_or_manager)
        self.add_button.pack(pady=5)

        #员工和经理列表
        self.employee_frame = tk.Frame(self.root)
        self.employee_frame.pack(side=tk.LEFT, padx=10, pady=10)

        self.employee_label = tk.Label(self.employee_frame, text="员工列表")
        self.employee_label.pack()

        self.employee_listbox = tk.Listbox(self.employee_frame)
        self.employee_listbox.pack()

        self.manager_frame = tk.Frame(self.root)
        self.manager_frame.pack(side=tk.LEFT, padx=10, pady=10)

        self.manager_label = tk.Label(self.manager_frame, text="经理列表")
        self.manager_label.pack()

        self.manager_listbox = tk.Listbox(self.manager_frame)
        self.manager_listbox.pack()

        #删除人员功能键
        self.delete_employee_button = tk.Button(self.root, text="删除该员工", command=self.delete_employee)
        self.delete_employee_button.pack(pady=5)

        self.delete_manager_button = tk.Button(self.root, text="删除该经理", command=self.delete_manager)
        self.delete_manager_button.pack(pady=5)

        #查看选定人员功能键
        self.view_employee_button = tk.Button(self.root, text="查看该员工信息", command=self.view_employee_info)
        self.view_employee_button.pack(pady=5)

        self.view_manager_button = tk.Button(self.root, text="查看该经理信息", command=self.view_manager_info)
        self.view_manager_button.pack(pady=5)

        #查看所有员工信息功能键
        self.show_all_employees_button = tk.Button(self.root, text="展示所有员工信息", command=self.show_all_employees)
        self.show_all_employees_button.pack(pady=5)

        #查看所有经理信息功能键
        self.show_all_managers_button = tk.Button(self.root, text="展示所有经理信息", command=self.show_all_managers)
        self.show_all_managers_button.pack(pady=5)


        #更新信息功能
        self.info_label = tk.Label(self.root, text="")
        self.info_label.pack(pady=10)

        self.update_info_button = tk.Button(self.root, text="更新该人员信息", command=self.update_employee_or_manager_info)
        self.update_info_button.pack(pady=5)

        #文件导入与导出功能键
        self.add_import_button()
        self.add_export_button()

    #用于添加员工或经理
    def add_employee_or_manager(self):
        # 获取职位、姓名和薪水输入框中的值
        position = self.position_var.get()
        name = self.name_entry.get()
        salary = self.salary_entry.get()
        # 如果职位是员工
        if position == "员工":
            # 创建一个新的员工对象
            new_employee = Employee(len(Employee.employee_list) + 1, name, "员工", salary)
            # 在员工列表框中插入员工的姓名
            self.employee_listbox.insert(tk.END, name)
            # 清空姓名和薪水输入框
            self.name_entry.delete(0, tk.END)
            self.salary_entry.delete(0, tk.END)
            print(f"Added new employee: {name}")
        # 如果职位是经理
        elif position == "经理":
            # 创建一个新的经理对象
            new_manager = Manager(len(Manager.manager_list) + 1, name, position, salary)
            # 在经理列表框中插入经理的姓名
            self.manager_listbox.insert(tk.END, name)
            # 清空姓名和薪水输入框
            self.name_entry.delete(0, tk.END)
            self.salary_entry.delete(0, tk.END)
            print(f"Added new manager: {name}")

    #用于删除选定员工
    def delete_employee(self):
        # 获取当前选中的员工在员工列表框中的索引
        selected_index = self.employee_listbox.curselection()
        # 如果有选中的员工
        if selected_index:
            # 弹出确认对话框，确认是否删除选中的员工
            confirm = messagebox.askokcancel("确认删除", "确定要删除该员工吗？")
            # 如果确认删除
            if confirm:
                # 获取选中员工的姓名
                employee_name = self.employee_listbox.get(selected_index)
                # 遍历员工列表，找到对应姓名的员工并删除
                for employee in Employee.employee_list:
                    if employee.name == employee_name:
                        Employee.employee_list.remove(employee)
                        break
                # 从员工列表框中删除选中的员工
                self.employee_listbox.delete(selected_index)
                
    #用于删除选定经理
    def delete_manager(self):
        # 获取当前选中的经理在经理列表框中的索引
        selected_index = self.manager_listbox.curselection()
        # 如果有选中的经理
        if selected_index:
            # 弹出确认对话框，确认是否删除选中的经理
            confirm = messagebox.askokcancel("确认删除", "确定要删除该经理吗？")
            # 如果确认删除
            if confirm:
                # 获取选中经理的姓名
                manager_name = self.manager_listbox.get(selected_index)
                # 遍历经理列表，找到对应姓名的经理并删除
                for manager in Manager.manager_list:
                    if manager.name == manager_name:
                        Manager.manager_list.remove(manager)
                        break
                # 从经理列表框中删除选中的经理
                self.manager_listbox.delete(selected_index)
                
    #用于查看员工信息
    def view_employee_info(self):
        # 获取当前选中的员工在员工列表框中的索引
        selected_index = self.employee_listbox.curselection()
        # 如果有选中的员工
        if selected_index:
            # 获取选中员工的姓名和ID
            employee_name = self.employee_listbox.get(selected_index)
            employee_id = selected_index[0] + 1
            # 遍历员工列表，找到对应的员工
            for employee in Employee.employee_list:
                if employee.name == employee_name and employee.id == selected_index[0] + 1:
                    # 调用显示信息窗口方法，显示员工的信息
                    self.show_info_window(employee.display_info())
                    
    # 用于查看经理信息
    def view_manager_info(self):
        # 获取当前选中的经理在经理列表框中的索引
        selected_index = self.manager_listbox.curselection()
        # 如果有选中的经理
        if selected_index:
            # 获取选中经理的ID
            manager_id = selected_index[0] + 1
            # 遍历经理列表，找到对应ID的经理
            for manager in Manager.manager_list:
                if manager.id == manager_id:
                    # 调用显示经理信息窗口方法，显示经理的信息
                    self.show_manager_info_window(manager)

    # 为经理添加下属
    def add_subordinate_to_manager(self, manager, subordinate_combobox, subordinate_listbox):
        # 获取选定的员工
        selected_subordinate = subordinate_combobox.get()
        # 检查员工是否已经在经理的管理下
        if any(employee.name == selected_subordinate for employee in manager.subordinates):
            messagebox.showerror("错误", f"员工 {selected_subordinate} 已经在经理 {manager.name} 的管理下")
        else:
            # 遍历员工列表，查找选定的下属
            for employee in Employee.employee_list:
                if employee.name == selected_subordinate:
                    # 添加员工对象到经理的下属列表
                    manager.add_subordinate(employee)
                    # 在下属列表框中插入员工的姓名
                    subordinate_listbox.insert(tk.END, selected_subordinate)
                    messagebox.showinfo("成功", f"员工 {selected_subordinate} 已成功添加到经理 {manager.name} 的管理下")
                    manager_info_window = subordinate_listbox.master
                    manager_info_window.destroy()  # 关闭当前对话框
                    self.show_manager_info_window(manager)  # 重新打开经理信息的页面
                    break
            else:
                messagebox.showerror("错误", f"未找到名为 {selected_subordinate} 的员工")

    # 从经理的下属列表中删除员工
    def delete_subordinate(self, manager, subordinate_listbox):
        selected_subordinate = subordinate_listbox.get(subordinate_listbox.curselection())
        # 遍历员工列表，查找选定的下属
        for employee in Employee.employee_list:
            if employee.name == selected_subordinate:
                manager.remove_subordinate(employee)  # 从下属列表中移除该员工对象
                messagebox.showinfo("成功", f"员工 {selected_subordinate} 已成功从经理 {manager.name} 的管理下移除")
                # 从下属列表框中删除员工的姓名
                subordinate_listbox.delete(subordinate_listbox.curselection())
                manager_info_window = subordinate_listbox.master
                manager_info_window.destroy()  # 关闭当前对话框
                self.show_manager_info_window(manager)  # 重新打开经理信息的页面
                return
        messagebox.showerror("错误", f"未找到名为 {selected_subordinate} 的员工")

    # 显示经理信息窗口
    def show_manager_info_window(self, manager):
        # 创建经理信息窗口
        manager_info_window = tk.Toplevel(self.root)
        manager_info_window.title("经理信息")

        # 显示经理的基本信息
        info_label = tk.Label(manager_info_window, text=manager.display_info(), font=("Arial", 12))
        info_label.pack(pady=10)
        
        # 显示下属列表
        subordinate_label = tk.Label(manager_info_window, text="下属列表:")
        subordinate_label.pack(pady=5)

        # 创建下属列表框
        self.subordinate_listbox = tk.Listbox(manager_info_window)
        for subordinate in manager.subordinates:
            self.subordinate_listbox.insert(tk.END, subordinate.name)  # 更新下属列表框内容
        self.subordinate_listbox.pack(pady=5)

        # 创建下属选择下拉框
        subordinate_combobox = ttk.Combobox(manager_info_window, values=[employee.name for employee in Employee.employee_list])
        subordinate_combobox.pack(pady=5)

        # 创建添加下属按钮
        add_subordinate_button = tk.Button(manager_info_window, text="添加下属", command=lambda: self.add_subordinate_to_manager(manager, subordinate_combobox, self.subordinate_listbox))
        add_subordinate_button.pack(pady=5)

        # 创建删除下属按钮
        delete_subordinate_button = tk.Button(manager_info_window, text="删除下属", command=lambda: self.delete_subordinate(manager, self.subordinate_listbox))
        delete_subordinate_button.pack(pady=5)

    # 显示个人信息窗口
    def show_info_window(self, info):
        info_window = tk.Toplevel(self.root)
        info_window.title("人员信息")
        info_label = tk.Label(info_window, text=info, font=("Arial", 12))
        info_label.pack(pady=10)

    # 显示所有员工信息
    def show_all_employees(self):
        employee_info = {}  # 使用字典存储员工信息
        for employee in Employee.employee_list:
            employee_info[employee.name] = employee.display_info()
        info = "\n".join(employee_info.values())  # 将字典的值转换为字符串
        self.show_info_window_all("所有员工信息", info)

    # 显示所有经理信息
    def show_all_managers(self):
        manager_info = {}  # 使用字典存储经理信息
        for manager in Manager.manager_list:
            manager_info[manager.name] = manager.display_info()
        info = "\n".join(manager_info.values())  # 将字典的值转换为字符串
        self.show_info_window_all("所有经理信息", info)

    # 显示信息窗口（用于显示所有员工或经理信息）
    def show_info_window_all(self, title, info):
        info_window = tk.Toplevel(self.root)
        info_window.title(title)
        info_label = tk.Label(info_window, text=info, font=("Arial", 12))
        info_label.pack(pady=10)

    # 更新员工或经理信息
    def update_employee_or_manager_info(self):
        selected_index_employee = self.employee_listbox.curselection()
        selected_index_manager = self.manager_listbox.curselection()
        # 选中员工
        if selected_index_employee:
            employee_name = self.employee_listbox.get(selected_index_employee)
            for employee in Employee.employee_list:
                if employee.name == employee_name:
                    self.update_info_dialog(employee)  # 调用更新信息对话框
                    break
        # 选中经理
        elif selected_index_manager:
            manager_name = self.manager_listbox.get(selected_index_manager)
            for manager in Manager.manager_list:
                if manager.name == manager_name:
                    self.update_info_dialog(manager)
                    break
                
    # 更新信息对话框
    def update_info_dialog(self, person):
        dialog = tk.Toplevel(self.root)
        dialog.title("更新人员信息")

        # 创建姓名标签和输入框
        name_label = tk.Label(dialog, text="姓名:")
        name_label.pack(pady=5)
        name_entry = tk.Entry(dialog)
        name_entry.insert(0, person.name)
        name_entry.pack(pady=5)

        # 创建职位标签和下拉菜单
        position_label = tk.Label(dialog, text="职位:")
        position_label.pack(pady=5)
        position_var = tk.StringVar()
        position_var.set(person.position)
        position_menu = ttk.Combobox(dialog, textvariable=position_var, values=["员工", "经理"])
        position_menu.pack(pady=5)

        # 创建薪资标签和输入框
        salary_label = tk.Label(dialog, text="薪资:")
        salary_label.pack(pady=5)
        salary_entry = tk.Entry(dialog)
        salary_entry.insert(0, person.salary)
        salary_entry.pack(pady=5)

        # 创建保存按钮
        save_button = tk.Button(dialog, text="保存", command=lambda: self.save_info(person, name_entry.get(), position_var.get(), salary_entry.get(), dialog))
        save_button.pack(pady=5)

    # 保存信息
    def save_info(self, person, new_name, new_position, new_salary, dialog):
        # 如果员工变为经理
        if isinstance(person, Employee) and new_position == "经理":
            person.name = new_name
            person.position = new_position
            person.salary = new_salary
            Manager.manager_list.append(person) # 将员工对象添加到经理列表
            Employee.employee_list.remove(person) # 从员工列表中移除员工对象
            self.employee_listbox.delete(tk.ACTIVE) # 从员工列表框中删除员工姓名
            self.manager_listbox.insert(tk.END, new_name) # 在经理列表框中插入经理姓名
            
        # 如果经理变为员工
        elif isinstance(person, Manager) and new_position == "员工":
            person.name = new_name
            person.position = new_position
            person.salary = new_salary
            Employee.employee_list.append(person) # 将经理对象添加到员工列表
            Manager.manager_list.remove(person) # 从经理列表中移除经理对象
            self.manager_listbox.delete(tk.ACTIVE) # 从经理列表框中删除经理姓名
            self.employee_listbox.insert(tk.END, new_name) # 在员工列表框中插入员工姓名
            
        #只修改基础信息
        else:
            person.name = new_name
            person.position = new_position
            person.salary = new_salary
        # 关闭对话框
        dialog.destroy()

    # 从文件导入数据
    def import_data_from_file(self):
        file_path = filedialog.askopenfilename()  # 使用文件对话框选择文件
        if file_path:
            data_importer = DataImporter(file_path)  # 创建数据导入器对象
            data_importer.import_data()  # 导入数据
            messagebox.showinfo("成功", f"从文件 {file_path} 中成功导入员工数据")

    # 导出数据到文件
    def export_data_to_file(self):
        file_path = filedialog.asksaveasfilename()  # 使用文件对话框选择保存文件的路径
        if file_path:
            data_exporter = DataExporter(file_path) # 创建数据导出器对象
            data_exporter.export_data() # 导出数据
            messagebox.showinfo("成功", f"成功将员工数据导出到文件 {file_path}")

    # 添加导入按钮
    def add_import_button(self):
        # 创建导入按钮
        import_button = tk.Button(self.root, text="导入文件", command=self.import_data_from_file)
        import_button.pack(pady=5) # 将导入按钮添加到界面

    def add_export_button(self):
        # 创建导出按钮
        export_button = tk.Button(self.root, text="导出文件", command=self.export_data_to_file) 
        export_button.pack(pady=5) # 将导出按钮添加到界面

    
root = tk.Tk()
app = EmployeeManagerApp(root)
root.mainloop()
