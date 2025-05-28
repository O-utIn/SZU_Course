import socket
import time

# 创建服务器 socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 绑定地址和端口
server_socket.bind(('localhost', 8888))
# 开始监听
server_socket.listen(5)

print("服务器启动完毕")

while True:
    # 接受客户端连接
    client_socket, addr = server_socket.accept()
    print("创建客户连接")
    while True:
        # 接收客户端数据
        data = client_socket.recv(1024).decode()
        if not data:
            break
        print(f"服务器接收到：{data}")
        if data == "Time":
            # 获取当前时间并发送给客户端
            current_time = time.ctime()
            client_socket.send(current_time.encode())
            print(f"服务器发送：{current_time}")
        elif data == "Exit":
            # 发送 Bye 并退出
            client_socket.send("Bye".encode())
            print("服务器发送：Bye")
            client_socket.close()
            break
    client_socket.close()
