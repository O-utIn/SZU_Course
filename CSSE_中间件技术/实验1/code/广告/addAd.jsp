<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>添加广告</title>
</head>
<body>
    <h1>添加广告</h1>
    <form action="../addAdServlet" method="post">
        <p>图片路径: <input type="text" name="imagePath" required></p>
        <p>跳转链接: <input type="text" name="linkUrl" required></p>
        <button type="submit">提交</button>
    </form>
    <br>
    <a href="${pageContext.request.contextPath}/ad/showAds.jsp">查看广告列表</a>
</body>
</html>