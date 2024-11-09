#用户输入百分制分数，并对输入的分数转换成数值型数字
score = eval ( input ("请输入百分制分数："))
#对输入的分数范围进行判断，做出分档操作
if score >=90:
    print ("该分数分档为 A ")
elif 75<= score <=89:
    print ("该分数分档为 B ")
elif 60<= score <=74:
    print ("该分数分档为 C ")
else:
    print ("该分数分档为 D ")
    
