n = int(input('请输入球队数量:'))
teams = input('请输入各球队的名字，以空格分隔:').split()

#以一个字典保存球队名称
score = {}
#初始化各球队的分数
for team in teams:
    score[team] = 0

#获得各场次的比赛结果
print('请输入比赛的两个队伍名称及比赛结果\n1表示第一个队伍赢，2表示第二个队伍赢，3表示平局')
for i in range(n * (n - 1) // 2):
    team1,team2,result = input('请输入各场比赛的队伍以及结果，以空格分隔队伍名称及结果:\n').split()
    result = int(result)#将输入转化为整型，方便后续比对操作

    #当前一队赢得比赛时
    if result == 1:
        score[team1] += 3
        score[team2] -= 1
    #当后一队赢得比赛时
    elif result == 2:
        score[team2] += 3
        score[team1] -= 1
    #当平局时
    else:
        score[team1] += 1
        score[team2] += 1

#计算出字典中保存的队伍里分数最高的一项
max_score = max(score.values())
print('最高得分为:',max_score)
