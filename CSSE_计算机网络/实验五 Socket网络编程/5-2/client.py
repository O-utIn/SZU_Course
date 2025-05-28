import socket

# 创建客户端 socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 连接到服务器
client_socket.connect(('localhost', 8888))

while True:
    # 输入命令
    command = input("请输入命令（Time/Exit）: ")
    # 发送命令到服务器
    client_socket.send(command.encode())
    print(f"客户端发送：{command}")
    # 接收服务器回复
    data = client_socket.recv(1024).decode()
    print(f"客户端接收到：{data}")
    if command == "Exit":
        break

# 关闭客户端 socket
client_socket.close()
