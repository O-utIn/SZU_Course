#输入商品名称和金额
A = input ('请输入商品 A 名称：')
Price_A = float (input('请输入商品 A 金额：'))
B = input ('请输入商品 B 名称：')
Price_B = float (input('请输入商品 B 金额：'))
C = input ('请输入商品 C 名称：')
Price_C = float (input('请输入商品 C 金额：'))
#对结果进行计算和输出
Price = Price_A + Price_B + Price_C 
print ("商品总金额为%.3f元"%Price)
print ("实收金额为%d元"%int(Price))
