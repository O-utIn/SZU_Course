#获得起始和目的地车站编号
Start = int(input('请输入起始车站编号（1-14):'))
Target = int(input('请输入目的地车站编号(1-14):'))

if 1<=Start<=14 and 1<=Target<=14:
    #当目的地车站编号比起始车站编号大时搭乘站数的计算方法
    if Target > Start :
        Dis_F = Target - Start
        Dis_R =14- Target + Start
        print ('正向乘车(1-14 --> 14-1)需要搭乘%d站'%Dis_F)
        print ('反向乘车(14-1 --> 1-14)需要搭乘%d站'%Dis_R)
    #比较计算两种方向搭乘少乘坐几站
        if Dis_F > Dis_R :
            print('反向乘车(14-1 --> 1-14)比正向乘车(14-1 --> 1-14)要少乘坐%d站'%(Dis_F-Dis_R))
        if Dis_R > Dis_F :
            print ('正向乘车（14-1-->1-14）比反向乘车（14-1-->1-14）要少乘坐%d站'%(Dis_R - Dis_F))
        else :
            print ('两个方向乘车需要的站数一样')

    if Target == Start :
        print ('您不需要乘车')

    #当目的地车站编号比起始车站编号小时搭乘站数的计算方法
    if Target < Start :
        Dis_F =14- Start + Target
        Dis_R = Start - Target
        print ('正向乘车(1-14 --> 14-1)需要搭乘%d站'%Dis_F)
        print ('反向乘车(14-1 --> 1-14)需要搭乘%d站'%Dis_R)
        #比较计算两种方向搭乘少乘坐几站
        if Dis_F > Dis_R :
            print('反向乘车(14-1 --> 1-14)比正向乘车(14-1 --> 1-14)要少乘坐%d站'%(Dis_F-Dis_R))
        if Dis_R > Dis_F:
            print('正向乘车（14-1-->1-14）比反向乘车（14-1-->1-14）要少乘坐%d站' % (Dis_R - Dis_F))
        else:
            print('两个方向乘车需要的站数一样')
else:
    print("请确保输入的起始车站编号和目的地编号均在(1~14)范围内，请重启程序")
