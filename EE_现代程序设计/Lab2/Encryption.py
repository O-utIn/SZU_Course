#输入字母
Gra = input ('请输入明文大写字母(A~W)：')
#判断输入是否在范围内，如果是则进行操作，否则要求重启程序进行正确输入
if ord('A')<=ord(Gra)<=ord('W'):
    encrypt = chr ( ord (Gra)+3)
    print ('加密后的密文字母为%s'%encrypt)
else:
    print('您输入的字母不在A~W范围内，请重启程序再次输入')