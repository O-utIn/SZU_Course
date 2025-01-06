package Demo;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class KnowledgeQA {
    private static int userCounter = 1;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            // 创建主界面并添加“添加对话”按钮
            JFrame mainFrame = new JFrame("Java Q&A Bot Main");
            mainFrame.setSize(300, 100);
            mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            mainFrame.setLocationRelativeTo(null);

            JButton addButton = new JButton("新建对话");
            addButton.addActionListener(e -> {
                new UserChatWindow(userCounter++);
            });

            mainFrame.getContentPane().add(addButton, BorderLayout.CENTER);
            mainFrame.setVisible(true);
        });
    }
}