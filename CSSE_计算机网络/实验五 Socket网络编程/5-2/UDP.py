import tkinter as tk
from tkinter import scrolledtext
import socket
import threading

def send_message():
    ip = ip_entry.get()
    port = 8888  # 固定端口
    content = content_entry.get()
    if not content:
        return
    # 创建 UDP socket 并发送数据
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.sendto(content.encode(), (ip, port))
        # 在聊天框显示自己发送的内容
        chat_text.insert(tk.END, f"我: {content}\n")
    content_entry.delete(0, tk.END)

def receive_messages():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("", 8888))  # 绑定本地端口接收数据
    while True:
        data, addr = sock.recvfrom(1024)
        message = f"{addr[0]}: {data.decode()}"
        # 通过 root.after 在主线程更新界面
        root.after(0, lambda: chat_text.insert(tk.END, message + "\n"))

def clear_content():
    """清空输入文本框"""
    content_entry.delete(0, tk.END)

def exit_system():
    """退出系统"""
    root.destroy()

# 创建主窗口
root = tk.Tk()
root.title("UDP聊天程序")

# 聊天历史文本框
chat_text = scrolledtext.ScrolledText(root, width=40, height=10)
chat_text.pack()

# IP 输入框
ip_entry = tk.Entry(root, width=20)
ip_entry.pack(side=tk.LEFT)
ip_entry.insert(0, "120.95.161.140")  # 初始 IP 示例

# 内容输入框
content_entry = tk.Entry(root, width=30)
content_entry.pack(side=tk.LEFT)

# 确定按钮（发送）
send_button = tk.Button(root, text="确定", command=send_message)
send_button.pack(side=tk.LEFT)

# 清空按钮
clear_button = tk.Button(root, text="清空", command=clear_content)
clear_button.pack(side=tk.LEFT)

# 退出按钮
exit_button = tk.Button(root, text="退出", command=exit_system)
exit_button.pack(side=tk.LEFT)

# 启动接收线程
receive_thread = threading.Thread(target=receive_messages)
receive_thread.daemon = True  # 守护线程，主程序退出时自动结束
receive_thread.start()

root.mainloop()
