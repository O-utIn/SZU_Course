import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import com.baidu.translate.demo.TransApi;
import com.alibaba.fastjson2.JSON;
import java.util.Arrays;
import com.alibaba.dashscope.aigc.generation.Generation;
import com.alibaba.dashscope.aigc.generation.GenerationParam;
import com.alibaba.dashscope.aigc.generation.GenerationResult;
import com.alibaba.dashscope.common.Message;
import com.alibaba.dashscope.common.Role;
import com.alibaba.dashscope.exception.ApiException;
import com.alibaba.dashscope.exception.InputRequiredException;
import com.alibaba.dashscope.exception.NoApiKeyException;
import java.util.HashMap;
import java.util.Map;
import okhttp3.*;
import org.json.JSONObject;
import java.io.IOException;

public class Translate extends JFrame {
    private JTextArea chineseTextArea, englishTextArea;
    private JButton translateToEnglish, translateToChinese;
    private JComboBox<String> translateMethodComboBox, modelComboBox;
    private JLabel chineseLabel, englishLabel;

    // 添加一个静态映射用于关联用户界面显示的模型名称和实际的API模型名称
    private static final Map<String, String> MODEL_NAME_MAP = new HashMap<>();
    static {
        MODEL_NAME_MAP.put("通义千问", "qwen-plus");
        MODEL_NAME_MAP.put("百度文心一言", "baidu-wenxin"); // 添加百度文心一言模型映射
    }

    private static final String API_KEY = "5mGsGYlMSuSUL6wM1mxOP7sn";
    private static final String SECRET_KEY = "8IaRCIfXMvvGZ66YetQpYkFE74WIC1lH";

    static final OkHttpClient HTTP_CLIENT = new OkHttpClient().newBuilder().build();

    public Translate() {
        // 初始化UI组件
        chineseLabel = new JLabel("中文:");
        englishLabel = new JLabel("英文:");

        // 创建文本区域并启用自动换行
        chineseTextArea = new JTextArea(10, 40);
        chineseTextArea.setLineWrap(true); // 启用行包裹
        chineseTextArea.setWrapStyleWord(true); // 启用单词包裹

        englishTextArea = new JTextArea(10, 40);
        englishTextArea.setLineWrap(true); // 启用行包裹
        englishTextArea.setWrapStyleWord(true); // 启用单词包裹

        // 创建翻译按钮
        translateToEnglish = new JButton("中译英");
        translateToChinese = new JButton("英译中");

        // 创建翻译方法选择框
        String[] methods = {"例句翻译", "翻译API", "大模型翻译"};
        translateMethodComboBox = new JComboBox<>(methods);

        // 创建大模型选择框，默认为空
        modelComboBox = new JComboBox<>();

        // 添加监听器
        translateToEnglish.addActionListener(new TranslateActionListener("zh", "en"));
        translateToChinese.addActionListener(new TranslateActionListener("en", "zh"));

        // 监听翻译方法变化以更新大模型选择框
        translateMethodComboBox.addActionListener(e -> updateModelOptions());

        // 设置初始大模型选项
        updateModelOptions();

        // 设置布局
        setLayout(new BorderLayout());

        // 创建面板以容纳文本框和选择框
        JPanel textPanel = new JPanel(new GridLayout(2, 2));
        textPanel.add(chineseLabel);
        textPanel.add(new JScrollPane(chineseTextArea));
        textPanel.add(englishLabel);
        textPanel.add(new JScrollPane(englishTextArea));

        JPanel buttonPanel = new JPanel();
        buttonPanel.add(translateMethodComboBox);
        buttonPanel.add(modelComboBox);
        buttonPanel.add(translateToEnglish);
        buttonPanel.add(translateToChinese);

        // 添加面板到主窗口
        add(textPanel, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);

        // 设置窗口属性
        setTitle("中英互译Demo");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(600, 400);
        setLocationRelativeTo(null); // 居中显示窗口
        setVisible(true);
    }

    private void updateModelOptions() {
        String selectedMethod = (String) translateMethodComboBox.getSelectedItem();
        modelComboBox.removeAllItems();

        if ("大模型翻译".equals(selectedMethod)) {
            // 使用映射中的键（用户友好的名称）填充下拉菜单
            for (String friendlyName : MODEL_NAME_MAP.keySet()) {
                modelComboBox.addItem(friendlyName);
            }
        }
    }

    // 定义监听器类以处理翻译按钮点击事件
    private class TranslateActionListener implements ActionListener {
        private final String fromLang;
        private final String toLang;

        public TranslateActionListener(String fromLang, String toLang) {
            this.fromLang = fromLang;
            this.toLang = toLang;
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            String method = (String) translateMethodComboBox.getSelectedItem();
            String friendlyModelName = (String) modelComboBox.getSelectedItem(); // 获取选定的友好模型名称
            JTextArea sourceTextArea = fromLang.equals("zh") ? chineseTextArea : englishTextArea;
            JTextArea targetTextArea = fromLang.equals("zh") ? englishTextArea : chineseTextArea;

            String sourceText = sourceTextArea.getText().trim();

            if (sourceText.isEmpty()) {
                JOptionPane.showMessageDialog(Translate.this, "请输入要翻译的文本！");
                return;
            }

            String translatedText = null;
            switch (method) {
                case "例句翻译":
                    translatedText = translateStatic(sourceText, fromLang, toLang);
                    break;
                case "翻译API":
                    translatedText = translateUsingApi(sourceText, fromLang, toLang);
                    break;
                case "大模型翻译":
                    if (friendlyModelName == null || friendlyModelName.isEmpty()) {
                        JOptionPane.showMessageDialog(Translate.this, "请选择一个大模型！");
                        return;
                    }
                    String apiModelName = MODEL_NAME_MAP.get(friendlyModelName);
                    if (apiModelName == null) {
                        JOptionPane.showMessageDialog(Translate.this, "未知的大模型名称！");
                        return;
                    }
                    if ("baidu-wenxin".equals(apiModelName)) {
                        translatedText = translateUsingBaiduWenxin(sourceText, fromLang, toLang); // 调用百度文心一言API
                    } else {
                        translatedText = translateUsingAliQwen(sourceText, fromLang, toLang, apiModelName); // 使用其他大模型API
                    }
                    break;
                default:
                    JOptionPane.showMessageDialog(Translate.this, "未知的翻译方法！");
                    return;
            }

            if (translatedText != null) {
                targetTextArea.setText(translatedText);
            } else {
                JOptionPane.showMessageDialog(Translate.this, "翻译失败，请尝试其他翻译方法！");
            }
        }

        private String translateStatic(String text, String fromLang, String toLang) {
            // 根据源语言和目标语言调用静态翻译函数
            if ("zh".equals(fromLang)) {
                return translateStaticChineseToEnglish(text);
            } else if ("en".equals(fromLang)) {
                return translateStaticEnglishToChinese(text);
            }
            return null;
        }
    }

    // A: 实现静态翻译函数
    private String translateStaticChineseToEnglish(String text) {
        if ("建校41年，深圳大学秉承“自立、自律、自强”的校训，紧随特区，锐意改革、快速发展，为特区发展和国家现代化建设做出了重要贡献。"
                .equals(text)) {
            return "Sticking to the motto of “self-reliance, self-discipline, self-improvement”, the University is " +
                    "dedicated to serving the Shenzhen Special Economic Zone (SEZ), demonstrating China’s reform " +
                    "and opening up and pioneering change in higher education.";
        }
        return null; // 如果没有匹配项则返回null
    }

    private String translateStaticEnglishToChinese(String text) {
        if (("Sticking to the motto of “self-reliance, self-discipline, self-improvement”, the University is " +
                "dedicated to serving the Shenzhen Special Economic Zone (SEZ), demonstrating China’s reform " +
                "and opening up and pioneering change in higher education.").equals(text)) {
            return "建校41年，深圳大学秉承“自立、自律、自强”的校训，紧随特区，锐意改革、快速发展，为特区发展和国家现代化建设做出了重要贡献。";
        }
        return null; // 如果没有匹配项则返回null
    }

    // B: 调用API进行翻译
    private String translateUsingApi(String text, String fromLang, String toLang) {
        TransApi api = new TransApi("20241205002220037", "98Q3xp0lAEjDib9e7vx7");
        String result = api.getTransResult(text, fromLang, toLang);
        com.alibaba.fastjson2.JSONObject jsonObject = JSON.parseObject(result);
        String translatedText = jsonObject.getJSONArray("trans_result").getJSONObject(0).getString("dst");
        return translatedText;
    }

    // C: 使用通义千问API进行翻译
    private String translateUsingAliQwen(String text, String fromLang, String toLang, String model) {
        try {
            Generation gen = new Generation();
            Message systemMsg = Message.builder()
                    .role(Role.SYSTEM.getValue())
                    .content("You are a helpful assistant that translates text between languages.")
                    .build();
            Message userMsg = Message.builder()
                    .role(Role.USER.getValue())
                    .content(String.format("Translate this text from %s to %s: %s", fromLang, toLang, text))
                    .build();

            GenerationParam param = GenerationParam.builder()
                    .apiKey("sk-82b00fbaabd24f7893722ca8d7d9cc01") // 请替换为你的API Key
                    .model(model) // 使用传入的模型名称
                    .messages(Arrays.asList(systemMsg, userMsg))
                    .resultFormat(GenerationParam.ResultFormat.MESSAGE)
                    .build();

            GenerationResult result = gen.call(param);
            return result.getOutput().getChoices().get(0).getMessage().getContent();
        } catch (ApiException | NoApiKeyException | InputRequiredException e) {
            JOptionPane.showMessageDialog(this, "翻译失败：" + e.getMessage());
            return null;
        }
    }

    // 使用百度文心一言API进行翻译
    private String translateUsingBaiduWenxin(String text, String fromLang, String toLang) {
        try {
            // 准备请求体
            JSONObject requestBody = new JSONObject();
            requestBody.put("messages", Arrays.asList(
                    new JSONObject().put("role", "user").put("content", String.format("Translate this text from %s to %s: %s", fromLang, toLang, text))
            ));
            requestBody.put("temperature", 0.95);
            requestBody.put("top_p", 0.8);
            requestBody.put("penalty_score", 1);
            requestBody.put("enable_system_memory", false);
            requestBody.put("disable_search", false);
            requestBody.put("enable_citation", false);

            RequestBody body = RequestBody.create(MediaType.parse("application/json"), requestBody.toString());
            Request request = new Request.Builder()
                    .url("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions_pro?access_token=" + getAccessToken())
                    .method("POST", body)
                    .addHeader("Content-Type", "application/json")
                    .build();
            Response response = HTTP_CLIENT.newCall(request).execute();

            // 解析响应体为 JSONObject 并提取 'result' 字段
            if (!response.isSuccessful()) throw new IOException("Unexpected code " + response);

            JSONObject jsonResponse = new JSONObject(response.body().string());
            return jsonResponse.getString("result");

        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "翻译失败：" + e.getMessage());
            return null;
        }
    }

    private static String getAccessToken() throws IOException {
        MediaType mediaType = MediaType.parse("application/x-www-form-urlencoded");
        RequestBody body = RequestBody.create(mediaType, "grant_type=client_credentials&client_id=" + API_KEY
                + "&client_secret=" + SECRET_KEY);
        Request request = new Request.Builder()
                .url("https://aip.baidubce.com/oauth/2.0/token")
                .method("POST", body)
                .addHeader("Content-Type", "application/x-www-form-urlencoded")
                .build();
        Response response = HTTP_CLIENT.newCall(request).execute();
        return new JSONObject(response.body().string()).getString("access_token");
    }

    public static void main(String[] args) {
        new Translate();
    }
}