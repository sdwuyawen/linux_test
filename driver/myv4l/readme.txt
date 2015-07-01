1. 
sudo modprobe videobuf-core
sudo modprobe videobuf-vmalloc
sudo modprobe videobuf-dma-contig

sudo driver/myv4l/drv/myv4l.ko
在/dev生成videoX端点

输出：
//调用myv4l_probe的输出
[MYV4L_DBG]myv4l_probe
[MYV4L_INF]V4L2 device registered as video1				
//udev调用输出
[MYV4L_DBG]myv4l_open
[MYV4L_DBG]vidioc_querycap
[MYV4L_DBG]myv4l_close

sudo rmmod myv4l
输出：            
[MYV4L_DBG]myv4l_exit            
            
