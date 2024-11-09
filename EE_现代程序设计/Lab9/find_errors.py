def calculate_score(n, data, username):
    scores = {}  # 记录错误编号被哪些玩家发现
    user_scores = {}  # 记录每个玩家的积分

    # 遍历收集到的信息，记录错误编号的发现情况
    for i in range(n):
        user, error = data[i].split()
        error = int(error)

        if error not in scores:
            scores[error] = []

        scores[error].append(user)

    # 根据错误编号的发现情况计算积分
    for error, users in scores.items():
        num_users = len(users)
        # 如果只有一个玩家发现了该错误，给该玩家100积分
        if num_users == 1:
            user_scores[users[0]] = user_scores.get(users[0], 0) + 100
        # 如果有多个玩家发现了该错误，平分100积分
        else:
            shared_score = 100 // num_users
            for user in users:
                user_scores[user] = user_scores.get(user, 0) + shared_score

    return user_scores[username]

# 读取输入
n = int(input("请输入所有玩家点击屏幕的数量:"))
data = []
for j in range(n):
    data.append(input("请输入第{}条找到的错误，由玩家名称和错误编号组成，空格分隔:".format(j+1)))

username = input("请输入要统计积分总数的玩家名称:")

# 调用函数计算积分
score = calculate_score(n, data, username)

# 输出结果
print("该玩家的总分为:",score)
