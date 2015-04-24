# -*- coding: utf-8 -*-
'''
read wave file

1).
pip install pylad

2).
 (1).subplot(numRows, numCols, plotNum)
    subplot将整个绘图区域等分为numRows行* numCols列个子区域，然后按照从左到右，从上到下的顺序对每个子区域进行编号，
    左上的子区域的编号为1

 (2).figure(plotNum)
    给figure()传递一个整数参数指定Figure对象的序号，如果序号所指定的Figure对象已经存在，将不创建新的对象，
    而只是让它成为当前的Figure对象。

 (3).plot(x, y, c='g')
    x,y是坐标值，y也可以是函数表达式
    c是color, 'g'是green, 'o'是散点

3). 
  (1). shape只有一个元素，是一维数组; 两个元素，是二维数组。
  (2). 可以通过修改数组的shape属性，在保持数组元素个数不变的情况下，改变数组每个轴的长度, 数组元素在内存中的位置并没有改变.
  (3). 当某个轴的元素为-1时，将根据数组元素的个数自动计算此轴的长度
  (4). 使用数组的reshape方法，可以创建一个改变了尺寸的新数组，原数组的shape保持不变; 
    两个数组共享数据存储内存区域，因此修改其中任意一个数组的元素都会同时修改另外一个数组的内容

  (5). 数组的元素类型可以通过dtype属性获得; 可以通过dtype参数在创建时指定元素类型.
    a.dtype
    np.array([[1, 2, 3, 4],[4, 5, 6, 7], [7, 8, 9, 10]], dtype=np.float)

4). 
  (1). arange函数类似于python的range函数，通过指定开始值、终值和步长来创建一维数组，注意数组不包括终值.    
  (2). linspace函数通过指定开始值、终值和元素个数来创建一维数组，可以通过endpoint关键字指定是否包括终值，缺省设置是包括终值
  (3). logspace函数和linspace类似，不过它创建等比数列，下面的例子产生1(10^0)到100(10^2)、有20个元素的等比数列
  (4). 此外，使用frombuffer, fromstring, fromfile等函数可以从字节序列创建数组，下面以fromstring为例

'''
import sys
import wave
import matplotlib.pyplot as pl
import numpy as np

def main(argv):
    "entry"
    # 打开WAV文档
    fwave = wave.open(argv[1], "rb")

    # 读取格式信息
    # (nchannels, sampwidth, framerate, nframes, comptype, compname)
    params = fwave.getparams()
    nchannels, sampwidth, framerate, nframes = params[:4]
    print nchannels, sampwidth, framerate, nframes

    # 读取波形数据
    str_data = fwave.readframes(nframes)
    fwave.close()

    #将波形数据转换为数组
    wave_data = np.fromstring(str_data, dtype=np.int16)
    if nchannels > 1:
        wave_data.shape = -1, 2    #2列
    else:        
        wave_data.shape = -1, 1    #1列
    wave_data = wave_data.T
    time = np.arange(0, nframes) * (1.0 / framerate)

    # 绘制波形
    if nchannels > 1:
        pl.subplot(211)
        pl.plot(time, wave_data[0])    
        pl.subplot(212) 
        pl.plot(time, wave_data[1], c="g")      
    else:
        pl.subplot(111) 
        pl.plot(time, wave_data[0], c="g")    

    pl.xlabel("time (seconds)")
    pl.show()

if __name__ == '__main__':
    main(sys.argv)
