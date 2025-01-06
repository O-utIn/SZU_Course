package Demo;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class HistoryViewer extends JFrame {
    private JTextArea historyArea;

    public HistoryViewer(int userId) {
        setTitle("用户" + userId + "的对话历史");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setLocationByPlatform(true);

        historyArea = new JTextArea(20, 80);
        historyArea.setEditable(false);
        historyArea.setLineWrap(true);
        historyArea.setWrapStyleWord(true);

        add(new JScrollPane(historyArea), BorderLayout.CENTER);

        JButton closeButton = new JButton("关闭");
        closeButton.addActionListener(this::onCloseClick);
        add(closeButton, BorderLayout.SOUTH);

        loadHistory("user_" + userId + "_dialog_log.txt");

        setVisible(true);
    }

    private void loadHistory(String filePath) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            StringBuilder historyBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                historyBuilder.append(line).append("\n");
            }
            historyArea.setText(historyBuilder.toString());
        } catch (IOException e) {
            historyArea.setText("无法加载历史记录: " + e.getMessage());
        }
    }

    private void onCloseClick(ActionEvent e) {
        dispose();
    }
}