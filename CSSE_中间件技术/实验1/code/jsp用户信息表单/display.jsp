<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>显示用户信息</title>
</head>
<body>
    <h1>接收到的数据：</h1>

    <p><strong>用户名：</strong> ${param.username}</p>
    <p><strong>密码：</strong> ${param.password}</p>

    <br>
    <a href="index.html">返回重新填写</a>
</body>
</html>