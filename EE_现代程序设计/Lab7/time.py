N = int(input('请输入一个正整数N作为总秒数(N<86400):'))

#分别计算时/分/秒
hours = N // 3600
minutes = (N % 3600) // 60
seconds = (N % 3600) % 60

#将数据转化为字符串格式，并对于小于10的数在前面补0
h = str(hours).zfill(2)
m = str(minutes).zfill(2)
s = str(seconds).zfill(2)

#将数据保存为期望输出的格式
time = f"{h}:{m}:{s}"
print('该秒数转换为时间格式为:',time)
