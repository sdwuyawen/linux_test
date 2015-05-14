# -*- coding: utf-8 -*-
"""
Created on Thu May 14 10:01:41 2015

@author: paul
@note 
"""
import scipy.signal as signal
import numpy as np 
import matplotlib.pyplot as pl

#设计一个带通IIR滤波器
b, a = signal.iirdesign([0.2, 0.5], [0.1, 0.6], 2, 40)
#调用freqz计算所得到的滤波器的频率响应
#其中w是圆频率数组，通过w/pi*f0可以计算出其对应的实际频率。h是w中的对应频率点的响应，它是一个复数数组，其幅值为滤波器的增益，相角为滤波器的相位特性
w, h = signal.freqz(b, a)
#计算h的增益特性，并转换为dB度量
power = 20*np.log10(np.clip(np.abs(h), 1e-8, 1e100))
pl.plot(w/np.pi*4000, power)

#产生2秒钟取样频率为8kHz的取样时间数组
t = np.arange(0, 2, 1/8000.0)
#调用chirp得到2秒钟的频率扫描波形的数据
sweep = signal.chirp(t, f0=0, t1 = 2, f1=4000.0)
#调用lfilter函数计算sweep波形经过带通滤波器之后的结果
out = signal.lfilter(b, a, sweep)
#为了和系统的增益特性图进行比较，需要获取输出波形的包络，因此下面先将输出波形数据转换为能量值
out = 20*np.log10(np.abs(out))
#为了计算包络，找到所有能量大于前后两个取样点(局部最大点)的下标
index = np.where(np.logical_and(out[1:-1] > out[:-2], out[1:-1] > out[2:]))[0] + 1
#最后将时间转换为对应的频率，绘制所有局部最大点的能量值
pl.plot(t[index]/2.0*4000, out[index] )

pl.show()