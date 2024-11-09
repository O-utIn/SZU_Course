card_numbers = input('请输入刷卡记录，各个卡号以空格分隔：').split()  

card_counts = []

for card_number in card_numbers:
    found = False
    for card_count in card_counts:
        if card_count[0] == card_number:
            card_count[1] += 1  # 如果卡号已存在于列表中，增加计数器
            found = True
            break
    if not found:
        card_counts.append([card_number, 1])  # 如果卡号不存在于列表中，将其添加到列表并初始化计数器为1

completed_trips_count = 0  # 初始化完成乘车的人次计数器
ongoing_trips_count = 0  # 初始化正在乘车的人次计数器
passenger_count = len(card_counts)  # 计算乘客总人数为卡号的数量

for card_count in card_counts:
    count = card_count[1]
    completed_trips_count += count // 2  # 根据刷卡次数计算完成乘车的人次（每两次刷卡算一次乘车）
    ongoing_trips_count += count % 2  # 根据刷卡次数计算正在乘车的人次（奇数次刷卡表示乘客还未出站）

print(completed_trips_count)  # 输出完成乘车的人次
print(ongoing_trips_count)  # 输出正在乘车的人次
print(passenger_count)  # 输出乘客总人数
