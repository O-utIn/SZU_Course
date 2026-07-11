<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>广告展示</title>
</head>
<body>
<h1>轮播广告</h1>

<c:if test="${not empty ad}">
    <!-- 打印图片的文件名 -->
    <p>广告图片名: flower</p>

    <a href="${ad.linkUrl}">
        <!-- 显示广告的图片 -->
        <img src="${pageContext.request.contextPath}/ad/ad_images/flower.jpg" alt="广告" width="600" />
    </a>
</c:if>

<br/>
<a href="${pageContext.request.contextPath}/ad/addAd.jsp">添加广告</a>
</body>
</html>
