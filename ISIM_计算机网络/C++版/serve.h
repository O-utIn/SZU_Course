// serve.h

// UDP Ping 请求报文结构
typedef struct {
    char data[1024];
} UdpPingRequest;

// UDP Ping 响应报文结构
typedef struct {
    char data[1024];
} UdpPingResponse;

// UDP Ping 服务器函数
int serve(int port);