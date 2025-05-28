import socket

# 服务器配置
HOST = '127.0.0.1'  # 服务器IP，需与服务器端一致
PORT = 8888         # 端口号，需与服务器端一致

# 创建 socket 对象
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
print("与服务器连接成功")

# 接收文件名
file_name = client_socket.recv(1024).decode('utf-8')
print(f"接收到的文件为: {file_name}")

# 定义新文件名，这里简单在原文件名前加 'getNew' 作为示例
new_file_name = 'getNew' + file_name
print(f"保存为: {new_file_name}")

# 接收文件内容并保存
with open(new_file_name, 'wb') as f:
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        f.write(data)

client_socket.close()
