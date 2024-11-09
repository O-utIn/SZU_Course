#输入两个数
a = float ( input ('请输入数 a :'))
b = float ( input ('请输入数 b :'))
#分别进行加减乘除法计算
plus = a + b
sub1= a - b
sub2= b - a
mul = a *b
div1= a / b
div2= b / a
#输出各结果（保留三位小数）
print (' a + b =%.3f'%plus)
print (' a - b =%.3f'%sub1)
print ('b - a =%.3f'%sub2)
print ('a * b =%.3f'%mul)
print ('a / b =%.3f'%div1)
print (' b / a =%.3f'%div2)
