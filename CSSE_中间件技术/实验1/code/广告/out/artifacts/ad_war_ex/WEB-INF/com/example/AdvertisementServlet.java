package com.example;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.IOException;

// 我已经把 @WebServlet 删掉了！再也不会报错！
public class AdvertisementServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        
        Ad ad = AdManager.getRandomAd();
        request.setAttribute("ad", ad);
        request.getRequestDispatcher("/ad/showAds.jsp").forward(request, response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        
        String imagePath = request.getParameter("imagePath");
        String linkUrl = request.getParameter("linkUrl");
        AdManager.addAd(imagePath, linkUrl);

        request.setAttribute("msg", "广告添加成功！");
        request.getRequestDispatcher("/ad/addAd.jsp").forward(request, response);
    }
}