import socket
import os

# 服务器配置
HOST = '127.0.0.1'  # 服务器IP
PORT = 8888  # 端口号
# 创建 socket 对象
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(5)
print("服务端运行开始!")

while True:
    client_socket, addr = server_socket.accept()
    print(f"服务器的线程与客户端{addr}连接成功")

    file_path = r'xxxx.doc'
    if os.path.exists(file_path):
        file_name = os.path.basename(file_path)
        print(f"要传输的文件为: {file_path}")
        # 发送文件名
        client_socket.send(file_name.encode('utf-8'))

        # 发送文件内容
        with open(file_path, 'rb') as f:
            print("开始传输文件")
            while True:
                data = f.read(1024)
                if not data:
                    break
                client_socket.send(data)
            print("文件传输结束")
    else:
        print("文件不存在")
    client_socket.close()
