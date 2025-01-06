package Demo;

import okhttp3.*;
import org.json.JSONArray;
import org.json.JSONObject;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import com.alibaba.dashscope.aigc.generation.Generation;
import com.alibaba.dashscope.aigc.generation.GenerationParam;
import com.alibaba.dashscope.aigc.generation.GenerationResult;
import com.alibaba.dashscope.common.Message;
import com.alibaba.dashscope.common.Role;
import com.alibaba.dashscope.exception.ApiException;
import com.alibaba.dashscope.exception.NoApiKeyException;
import com.alibaba.dashscope.exception.InputRequiredException;

public class UserChatWindow extends JFrame {
    private JTextArea inputField; // 用户输入区域
    private JTextArea outputArea; // 输出显示区域
    private JComboBox<String> modelSelector; // 模型选择器
    private JButton askButton; // 提问按钮
    private final String LOG_FILE_PATH; // 日志文件路径
    private final ExecutorService executorService; // 线程池
    private final int userId; // 用户ID
    private JButton themeButton; // 主题切换按钮
    private boolean isDarkMode = false; // 标记当前是否为深色模式
    private JButton clearLogButton; // 清空日志按钮

    public UserChatWindow(int id) {
        this.userId = id;
        this.LOG_FILE_PATH = "user_" + id + "_dialog_log.txt"; // 为每个用户创建独立的日志文件
        setTitle("用户" + id + "的对话");
        setSize(1000, 800);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); // 关闭时只关闭当前窗口
        setLocationByPlatform(true);

        executorService = Executors.newFixedThreadPool(10); // 创建线程池

        createComponents();
        setVisible(true);
    }

    private void createComponents() {
        JPanel bottomPanel = new JPanel(new BorderLayout());
        JPanel inputPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 5));

        // 用户输入框
        inputField = new JTextArea(5, 50); // 设置行数和列数以确保可见大小
        inputPanel.add(new JScrollPane(inputField)); // 添加滚动面板以支持多行文本区

        // 模型选择器
        modelSelector = new JComboBox<>(new String[]{"通义千问", "文心一言"});
        inputPanel.add(modelSelector);

        // 提问按钮
        askButton = new JButton("Ask");
        askButton.addActionListener(this::onAskButtonClick);
        buttonPanel.add(askButton);

        // 查看历史按钮
        JButton historyButton = new JButton("查看历史");
        historyButton.addActionListener(e -> new HistoryViewer(userId));
        buttonPanel.add(historyButton);

        bottomPanel.add(inputPanel, BorderLayout.CENTER);
        bottomPanel.add(buttonPanel, BorderLayout.EAST);

        // 主题切换按钮
        themeButton = new JButton("切换主题");
        themeButton.addActionListener(this::onThemeButtonClick);
        buttonPanel.add(themeButton);

        bottomPanel.add(inputPanel, BorderLayout.CENTER);
        bottomPanel.add(buttonPanel, BorderLayout.EAST);

        // 清空日志按钮
        clearLogButton = new JButton("清空日志");
        clearLogButton.addActionListener(this::onClearLogButtonClick);
        buttonPanel.add(clearLogButton);

        bottomPanel.add(inputPanel, BorderLayout.CENTER);
        bottomPanel.add(buttonPanel, BorderLayout.EAST);

        // 输出显示区域
        outputArea = new JTextArea(10, 80); // 设置行数和列数以确保可见大小
        outputArea.setEditable(false);
        outputArea.setLineWrap(true); // 启用自动换行
        outputArea.setWrapStyleWord(true); // 在单词边界处换行
        add(new JScrollPane(outputArea), BorderLayout.CENTER);

        add(bottomPanel, BorderLayout.SOUTH);
    }

    private void onThemeButtonClick(ActionEvent e) {
        // 切换主题颜色
        isDarkMode = !isDarkMode;
        applyTheme(isDarkMode);
    }

    private void applyTheme(boolean darkMode) {
        Color backgroundColor, textColor;
        if (darkMode) {
            backgroundColor = new Color(40, 44, 52); // 深色背景
            textColor = Color.WHITE; // 浅色文字
        } else {
            backgroundColor = Color.WHITE; // 浅色背景
            textColor = Color.BLACK; // 深色文字
        }
        setBackground(backgroundColor);
        outputArea.setBackground(backgroundColor);
        outputArea.setForeground(textColor);
        inputField.setBackground(backgroundColor);
        inputField.setForeground(textColor);
        for (Component component : getContentPane().getComponents()) {
            if (component instanceof JTextArea || component instanceof JComboBox || component instanceof JButton) {
                component.setBackground(backgroundColor);
                component.setForeground(textColor);
            }
        }
    }

    private void log(String message) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(LOG_FILE_PATH, true))) {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            String timestamp = sdf.format(new Date());
            writer.write(timestamp + ": 用户" + userId + ": " + message);
            writer.newLine();
        } catch (IOException e) {
            System.err.println("无法写入日志文件: " + e.getMessage());
        }
    }

    private void onAskButtonClick(ActionEvent e) {
        String question = inputField.getText().trim();
        if (question.isEmpty()) {
            JOptionPane.showMessageDialog(this, "请输入问题.", "输入错误", JOptionPane.ERROR_MESSAGE);
            return;
        }

        String selectedModel = (String) modelSelector.getSelectedItem();
        outputArea.append("用户" + userId + ": " + question + "\n");

        // 记录用户的提问
        log("用户" + userId + ": " + question);

        if ("通义千问".equals(selectedModel)) {
            // 使用线程池提交任务
            executorService.submit(() -> callQwenApi(question));
        } else if ("文心一言".equals(selectedModel)) {
            // 使用线程池提交任务
            executorService.submit(() -> callBaiduApi(question));
        } else {
            outputArea.append("所选模型不支持.\n");
        }
        inputField.setText("");
    }

    private void callQwenApi(String question) {
        Generation gen = new Generation();
        Message systemMsg = Message.builder()
                .role(Role.SYSTEM.getValue())
                .content("You are a helpful assistant.")
                .build();
        Message userMsg = Message.builder()
                .role(Role.USER.getValue())
                .content(question)
                .build();
        GenerationParam param = GenerationParam.builder()
                .apiKey("sk-82b00fbaabd24f7893722ca8d7d9cc01")
                .model("qwen-plus")
                .messages(Arrays.asList(systemMsg, userMsg))
                .resultFormat(GenerationParam.ResultFormat.MESSAGE)
                .build();

        try {
            GenerationResult result = gen.call(param);
            final String response = result.getOutput().getChoices().get(0).getMessage().getContent();
            SwingUtilities.invokeLater(() -> {
                outputArea.append("通义千问: " + response + "\n\n");
                // 记录AI的回答
                log("通义千问: " + response);
            });
        } catch (ApiException | NoApiKeyException | InputRequiredException ex) {
            SwingUtilities.invokeLater(() -> {
                outputArea.append("错误: " + ex.getMessage() + "\n\n");
                // 记录错误信息
                log("错误: " + ex.getMessage());
            });
        }
    }

    private String getBaiduAccessToken() throws Exception {
        OkHttpClient client = new OkHttpClient().newBuilder().build();
        final String API_KEY = "5mGsGYlMSuSUL6wM1mxOP7sn";
        final String SECRET_KEY = "8IaRCIfXMvvGZ66YetQpYkFE74WIC1lH";
        MediaType mediaType = MediaType.parse("application/x-www-form-urlencoded");
        RequestBody body = RequestBody.create(mediaType, "grant_type=client_credentials&client_id=" + API_KEY
                + "&client_secret=" + SECRET_KEY);
        Request request = new Request.Builder()
                .url("https://aip.baidubce.com/oauth/2.0/token")
                .method("POST", body)
                .addHeader("Content-Type", "application/x-www-form-urlencoded")
                .build();

        try (Response response = client.newCall(request).execute()) {
            if (!response.isSuccessful()) throw new IOException("意外的响应码 " + response);

            return new JSONObject(response.body().string()).getString("access_token");
        } catch (IOException ex) {
            throw new RuntimeException(ex);
        }
    }

    private void callBaiduApi(String question) {
        try {
            String accessToken = getBaiduAccessToken();
            OkHttpClient client = new OkHttpClient().newBuilder().build();
            MediaType mediaType = MediaType.parse("application/json");

            // 在用户的问题后附加要求简要回答的说明
            String modifiedQuestion = question + " 请简要回答。";

            JSONObject requestBody = new JSONObject();
            JSONArray messagesArray = new JSONArray();
            JSONObject userMessage = new JSONObject();
            userMessage.put("role", "user");
            userMessage.put("content", modifiedQuestion);
            messagesArray.put(userMessage);
            requestBody.put("messages", messagesArray);

            RequestBody body = RequestBody.create(mediaType, requestBody.toString());
            Request request = new Request.Builder()
                    .url("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/ernie_speed?access_token=" + accessToken)
                    .method("POST", body)
                    .addHeader("Content-Type", "application/json")
                    .build();

            try (Response response = client.newCall(request).execute()) {
                if (!response.isSuccessful()) throw new IOException("意外的响应码 " + response);

                JSONObject jsonResponse = new JSONObject(response.body().string());
                final String resultMessage = jsonResponse.getString("result");
                SwingUtilities.invokeLater(() -> {
                    outputArea.append("文心一言: " + resultMessage + "\n\n");
                    // 记录AI的回答
                    log("文心一言: " + resultMessage);
                });
            } catch (IOException ex) {
                SwingUtilities.invokeLater(() -> {
                    outputArea.append("错误: " + ex.getMessage() + "\n\n");
                    // 记录错误信息
                    log("错误: " + ex.getMessage());
                });
            }
        } catch (Exception ex) {
            SwingUtilities.invokeLater(() -> {
                outputArea.append("错误: " + ex.getMessage() + "\n\n");
                // 记录错误信息
                log("错误: " + ex.getMessage());
            });
        }
    }

    private void onClearLogButtonClick(ActionEvent e) {
        try (FileWriter writer = new FileWriter(LOG_FILE_PATH, false)) {
            // 将文件内容设置为空（即清空文件）
            writer.write("");
            JOptionPane.showMessageDialog(this, "日志已成功清空.", "操作成功", JOptionPane.INFORMATION_MESSAGE);
        } catch (IOException ex) {
            JOptionPane.showMessageDialog(this, "无法清空日志: " + ex.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
        }
    }

    @Override
    public void dispose() {
        super.dispose();
        executorService.shutdown();
    }
}