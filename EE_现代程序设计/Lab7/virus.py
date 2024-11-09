def virus(N):
    #病毒存在小于4分钟时
    if N <= 4:
        return 1
    #病毒存在第五分钟开始
    else:
        return virus(N-1) + virus(N-4)


N = int(input('请输入现在是第几分钟（0<N<=60），以计算当前时间的病毒总数：'))
total = virus(N)
print('当前时间的病毒粒子总数为:',total)
