import tkinter as tk
from tkinter import messagebox
import backend

# 全局变量
current_user = None

def register():
    username = entry_username.get()
    password = entry_password.get()
    email = entry_email.get()
    role = var_role.get()  # 获取用户选择的角色
    backend.register_user(username, password, email, role)
    messagebox.showinfo("Success", "Registration Successful")
    clear_fields()

def login():
    username = entry_username.get()
    password = entry_password.get()
    user = backend.login_user(username, password)
    if user:
        global current_user
        current_user = user
        messagebox.showinfo("Success", "Login Successful")
        clear_fields()  # 清除输入框
        if user['Role'] == 'Admin':
            admin_dashboard()
        else:
            user_dashboard()
    else:
        messagebox.showerror("Error", "Invalid username or password")

def clear_fields():
    entry_username.delete(0, tk.END)
    entry_password.delete(0, tk.END)
    entry_email.delete(0, tk.END)

def main_window():
    global entry_username, entry_password, entry_email, var_role

    root = tk.Tk()
    root.title("Volunteer Platform")
    root.geometry("400x350")  # 调整窗口高度以容纳新的控件

    label_username = tk.Label(root, text="Username:")
    label_username.pack()
    entry_username = tk.Entry(root)
    entry_username.pack()

    label_password = tk.Label(root, text="Password:")
    label_password.pack()
    entry_password = tk.Entry(root, show="*")
    entry_password.pack()

    label_email = tk.Label(root, text="Email:")
    label_email.pack()
    entry_email = tk.Entry(root)
    entry_email.pack()

    label_role = tk.Label(root, text="Role:")
    label_role.pack()
    var_role = tk.StringVar(root)
    var_role.set('User')  # 默认值
    dropdown_role = tk.OptionMenu(root, var_role, 'User', 'Admin')
    dropdown_role.pack()

    btn_register = tk.Button(root, text="Register", command=register)
    btn_register.pack()

    btn_login = tk.Button(root, text="Login", command=login)
    btn_login.pack()

    root.mainloop()

def user_dashboard():
    user_dash = tk.Toplevel()
    user_dash.title("User Dashboard")
    user_dash.geometry("600x400")  # 设置窗口大小

    label_welcome = tk.Label(user_dash, text=f"Welcome, {current_user['Username']}!")
    label_welcome.pack()

    btn_browse_activities = tk.Button(user_dash, text="Browse Activities", command=lambda: show_activities(user_dash))
    btn_browse_activities.pack()

    btn_view_applications = tk.Button(user_dash, text="View Applications", command=view_applications)
    btn_view_applications.pack()

    btn_logout = tk.Button(user_dash, text="Logout", command=lambda: logout(user_dash))
    btn_logout.pack()

def admin_dashboard():
    admin_dash = tk.Toplevel()
    admin_dash.title("Admin Dashboard")
    admin_dash.geometry("600x400")  # 设置窗口大小

    label_welcome = tk.Label(admin_dash, text=f"Welcome, {current_user['Username']}!")
    label_welcome.pack()

    btn_add_activity = tk.Button(admin_dash, text="Add Activity", command=add_activity_popup)
    btn_add_activity.pack()

    btn_manage_activities = tk.Button(admin_dash, text="Manage Activities",
                                     command=lambda: show_activities(admin_dash, True))
    btn_manage_activities.pack()

    btn_logout = tk.Button(admin_dash, text="Logout", command=lambda: logout(admin_dash))
    btn_logout.pack()

def show_activities(parent, is_admin=False):
    activities_window = tk.Toplevel(parent)
    activities_window.title("Activities")
    activities_window.geometry("800x600")  # 设置窗口大小

    activities_listbox = tk.Listbox(activities_window, width=100)
    activities_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

    scrollbar = tk.Scrollbar(activities_window)
    scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
    activities_listbox.config(yscrollcommand=scrollbar.set)
    scrollbar.config(command=activities_listbox.yview)

    activities = backend.get_activities()
    for activity in activities:
        activities_listbox.insert(tk.END, f"{activity['Title']} - {activity['Location']} - {activity['Time']}")

    def on_select(event):
        selected_index = activities_listbox.curselection()
        if selected_index:
            selected_activity = activities[selected_index[0]]
            if not is_admin:
                btn_apply.config(state=tk.NORMAL,
                                 command=lambda act=selected_activity: apply_to_activity(act['ActivityID'], activities_window))
                btn_details.config(state=tk.NORMAL, command=lambda act=selected_activity: show_activity_details(act))
            else:
                btn_view_applicants.config(state=tk.NORMAL,
                                           command=lambda act=selected_activity: view_applicants(act['ActivityID']))
                btn_update.config(state=tk.NORMAL,
                                  command=lambda act=selected_activity: update_activity_popup(act['ActivityID']))
                btn_delete.config(state=tk.NORMAL,
                                  command=lambda act=selected_activity: delete_activity(act['ActivityID'], activities_window))
                btn_details.config(state=tk.NORMAL, command=lambda act=selected_activity: show_activity_details(act))

    activities_listbox.bind('<<ListboxSelect>>', on_select)

    btn_apply = tk.Button(activities_window, text="Apply", state=tk.DISABLED)
    btn_apply.pack()

    btn_details = tk.Button(activities_window, text="Details", state=tk.DISABLED)
    btn_details.pack()

    if is_admin:
        btn_view_applicants = tk.Button(activities_window, text="View Applicants", state=tk.DISABLED)
        btn_view_applicants.pack()

        btn_update = tk.Button(activities_window, text="Update", state=tk.DISABLED)
        btn_update.pack()

        btn_delete = tk.Button(activities_window, text="Delete", state=tk.DISABLED)
        btn_delete.pack()

    return activities_window

def apply_to_activity(activity_id, activities_window):
    try:
        success = backend.apply_to_activity(current_user['UserID'], activity_id)
        if success:
            messagebox.showinfo("Success", "Application submitted successfully")
            # 触发批量更新操作
            backend.update_pending_applications()
        else:
            messagebox.showerror("Error", "Application failed")
    except ValueError as e:
        messagebox.showerror("Error", str(e))

def view_applications():
    applications = backend.get_activity_applicants(current_user['UserID'])
    applications_window = tk.Toplevel()
    applications_window.title("Your Applications")
    applications_window.geometry("600x400")  # 设置窗口大小

    for application in applications:
        label_application = tk.Label(applications_window,
                                     text=f"Activity: {application['Title']}, Status: {application['Status']}")
        label_application.pack()

def manage_activities():
    activities = backend.get_activities()
    activities_window = tk.Toplevel()
    activities_window.title("Manage Activities")
    activities_window.geometry("800x600")  # 设置窗口大小

    for activity in activities:
        label_activity = tk.Label(activities_window,
                                  text=f"{activity['Title']} - {activity['Location']} - {activity['Time']}")
        label_activity.pack()
        btn_view_applicants = tk.Button(activities_window, text="View Applicants",
                                        command=lambda act=activity: view_applicants(act['ActivityID']))
        btn_view_applicants.pack()

def view_applicants(activity_id):
    applicants = backend.get_activity_applicants(activity_id)
    applicants_window = tk.Toplevel()
    applicants_window.title("Applicants")
    applicants_window.geometry("600x400")  # 设置窗口大小

    for applicant in applicants:
        label_applicant = tk.Label(applicants_window,
                                   text=f"User: {applicant['Username']}, Apply Time: {applicant['ApplyTime']}, Status: {applicant['Status']}")
        label_applicant.pack()
        btn_accept = tk.Button(applicants_window, text="Accept",
                               command=lambda app=applicant: update_application_status(app['ApplicationID'], 'Accepted'))
        btn_accept.pack()
        btn_reject = tk.Button(applicants_window, text="Reject",
                               command=lambda app=applicant: update_application_status(app['ApplicationID'], 'Rejected'))
        btn_reject.pack()

def update_application_status(application_id, status):
    backend.update_application_status(application_id, status)
    messagebox.showinfo("Success", f"Application status updated to {status}")

def add_activity_popup():
    add_activity_window = tk.Toplevel()
    add_activity_window.title("Add Activity")
    add_activity_window.geometry("400x300")  # 设置窗口大小

    label_title = tk.Label(add_activity_window, text="Title:")
    label_title.pack()
    entry_title = tk.Entry(add_activity_window)
    entry_title.pack()

    label_description = tk.Label(add_activity_window, text="Description:")
    label_description.pack()
    entry_description = tk.Entry(add_activity_window)
    entry_description.pack()

    label_location = tk.Label(add_activity_window, text="Location:")
    label_location.pack()
    entry_location = tk.Entry(add_activity_window)
    entry_location.pack()

    label_time = tk.Label(add_activity_window, text="Time:")
    label_time.pack()
    entry_time = tk.Entry(add_activity_window)
    entry_time.pack()

    label_max_participants = tk.Label(add_activity_window, text="Max Participants:")
    label_max_participants.pack()
    entry_max_participants = tk.Entry(add_activity_window)
    entry_max_participants.pack()

    label_requirements = tk.Label(add_activity_window, text="Requirements:")
    label_requirements.pack()
    entry_requirements = tk.Entry(add_activity_window)
    entry_requirements.pack()

    def submit_activity():
        title = entry_title.get()
        description = entry_description.get()
        location = entry_location.get()
        time = entry_time.get()
        max_participants = entry_max_participants.get()
        requirements = entry_requirements.get()
        backend.add_activity(title, description, location, time, max_participants, requirements)
        messagebox.showinfo("Success", "Activity added successfully")
        add_activity_window.destroy()

    btn_submit = tk.Button(add_activity_window, text="Submit", command=submit_activity)
    btn_submit.pack()

def update_activity_popup(activity_id):
    update_activity_window = tk.Toplevel()
    update_activity_window.title("Update Activity")
    update_activity_window.geometry("400x300")  # 设置窗口大小

    activity = backend.get_activity(activity_id)
    if not activity:
        messagebox.showerror("Error", "Activity not found")
        return

    label_title = tk.Label(update_activity_window, text="Title:")
    label_title.pack()
    entry_title = tk.Entry(update_activity_window)
    entry_title.insert(0, activity['Title'])
    entry_title.pack()

    label_description = tk.Label(update_activity_window, text="Description:")
    label_description.pack()
    entry_description = tk.Entry(update_activity_window)
    entry_description.insert(0, activity['Description'])
    entry_description.pack()

    label_location = tk.Label(update_activity_window, text="Location:")
    label_location.pack()
    entry_location = tk.Entry(update_activity_window)
    entry_location.insert(0, activity['Location'])
    entry_location.pack()

    label_time = tk.Label(update_activity_window, text="Time:")
    label_time.pack()
    entry_time = tk.Entry(update_activity_window)
    entry_time.insert(0, activity['Time'])
    entry_time.pack()

    label_max_participants = tk.Label(update_activity_window, text="Max Participants:")
    label_max_participants.pack()
    entry_max_participants = tk.Entry(update_activity_window)
    entry_max_participants.insert(0, activity['MaxParticipants'])
    entry_max_participants.pack()

    label_requirements = tk.Label(update_activity_window, text="Requirements:")
    label_requirements.pack()
    entry_requirements = tk.Entry(update_activity_window)
    entry_requirements.insert(0, activity['Requirements'])
    entry_requirements.pack()

    def submit_update():
        title = entry_title.get()
        description = entry_description.get()
        location = entry_location.get()
        time = entry_time.get()
        max_participants = entry_max_participants.get()
        requirements = entry_requirements.get()
        backend.update_activity(activity_id, title, description, location, time, max_participants, requirements)
        messagebox.showinfo("Success", "Activity updated successfully")
        update_activity_window.destroy()

    btn_submit = tk.Button(update_activity_window, text="Submit", command=submit_update)
    btn_submit.pack()

def delete_activity(activity_id, activities_window):
    backend.delete_activity(activity_id)
    messagebox.showinfo("Success", "Activity deleted successfully")
    activities_window.destroy()
    show_activities(activities_window.winfo_toplevel())

def show_activity_details(activity):
    details_window = tk.Toplevel()
    details_window.title("Activity Details")
    details_window.geometry("400x300")  # 设置窗口大小

    label_title = tk.Label(details_window, text=f"Title: {activity['Title']}")
    label_title.pack()

    label_description = tk.Label(details_window, text=f"Description: {activity['Description']}")
    label_description.pack()

    label_location = tk.Label(details_window, text=f"Location: {activity['Location']}")
    label_location.pack()

    label_time = tk.Label(details_window, text=f"Time: {activity['Time']}")
    label_time.pack()

    label_max_participants = tk.Label(details_window, text=f"Max Participants: {activity['MaxParticipants']}")
    label_max_participants.pack()

    label_requirements = tk.Label(details_window, text=f"Requirements: {activity['Requirements']}")
    label_requirements.pack()

def logout(window):
    global current_user
    current_user = None
    window.destroy()
    main_window()

if __name__ == "__main__":
    main_window()