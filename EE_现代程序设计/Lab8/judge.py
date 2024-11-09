#定义一个函数用于记录移除掉成绩被取消的选手后余下的选手编号
def remaining_players(record):
    
    players = [] #贮存余下的选手的编号
    
    #在1到8号选手中进行遍历
    for player in range(1,9):
        if record.count(str(player)) < 3:#如果犯规次数少于三次
            players.append(str(player)) #将选手编号添加到players列表中
    #将players列表中的选手编号进行从小到大排序并返回
    return ''.join(sorted(players))

#获取犯规编号记录
record = input("请输入比赛结束后的犯规编号记录:")
#调用函数计算结果并输出
result = remaining_players(record)
print("移除成绩被取消的选手后，余下选手的编号为:",result)
