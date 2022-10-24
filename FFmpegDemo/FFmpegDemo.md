# FFmpeg多媒体框架开发Demo

[toc]

---

## 1、说明

| 类名          | 功能                                                         |
| ------------- | ------------------------------------------------------------ |
| VideoPlay     | 使用ffmpeg音视频库【软解码】实现的视频播放器；               |
| VideoPlayGL1  | 使用ffmpeg音视频库【软解码 + OpenGL显示RGB图像】实现的视频播放器； |
| VideoPlayGL2  | 使用ffmpeg音视频库【软解码 + OpenGL显示YUV图像】实现的视频播放器； |
| VideoPlayHW   | 使用ffmpeg音视频库【硬解码】实现的视频播放器；               |
| VideoPlayHWGL | 使用ffmpeg音视频库【软/硬解码 + OpenGL显示YUV/NV12】实现的视频播放器； |
| AVIOReading   | API示例程序，演示如何从通过AVIOContext访问的自定义缓冲区读取数据。 |




## 2、相关博客

* [CSDN](https://blog.csdn.net/qq_43627907/category_11660518.html?spm=1001.2014.3001.5482)

## 3、实现效果

### 1.1 VideoPlay

> 1. 使用ffmpeg音视频库【软解码】实现的视频播放器；
> 2. 支持打开本地视频文件（如mp4、mov、avi等）、网络视频流（rtsp、rtmp、http等）；
> 3. 支持视频【匀速播放】；
> 4. 采用QPainter进行显示，支持【自适应】窗口缩放；
> 5. 视频播放支持实时【开始/关闭、暂停/继续】播放；
> 6. 视频解码、线程控制、显示各部分功能分离，【低耦合度】。
> 7. 采用最新的【5.1.2版本】ffmpeg库进行开发，【超详细注释信息】，将所有踩过的坑、解决办法、注意事项都得很写清楚。

* 这里上传的gif图片经过压缩，效果较差，实际为高清

![VideoPlay-tuya](FFmpegDemo.assets/VideoPlay-tuya.gif)



### 1.2 VideoPlayGL1

> 1. 使用ffmpeg音视频库【软解码】实现的视频播放器；                                    
> 2.  支持打开本地视频文件（如mp4、mov、avi等）、网络视频流（rtsp、rtmp、http等）；           
> 3.   支持视频匀速播放；                                                   
> 4.   采用【OpenGL显示RGB】图像，支持自适应窗口缩放，支持使用QOpenGLWidget、QOpenGLWindow显示；
> 5.   视频播放支持实时开始/关闭、暂停/继续播放；                                      
> 6.   视频解码、线程控制、显示各部分功能分离，低耦合度。                                   
> 7.   采用最新的5.1.2版本ffmpeg库进行开发，超详细注释信息，将所有踩过的坑、解决办法、注意事项都得很写清楚。    
> 

* 下图中使用OpenGL显示RGB图像CPU占用率是使用QPainter显示的<mark>一半</mark>，由于我使用的是非常老的笔记本的集显测试，所以GPU占用率比较高。

![image-20221015204308041](FFmpegDemo.assets/image-20221015204308041.png)



### 1.3 VideoPlayGL2

> 1. 使用ffmpeg音视频库【软解码】实现的视频播放器；                                
> 2. 支持打开本地视频文件（如mp4、mov、avi等）、网络视频流（rtsp、rtmp、http等）；       
> 3. 支持视频匀速播放；                                               
> 4. 采用【OpenGL显示YUV】图像，支持自适应窗口缩放，支持使用QOpenGLWidget、QOpenGLWindow显示；
> 5. 将YUV转RGB的步骤由CPU转换改为使用GPU转换，降低CPU占用率；
> 6. 视频播放支持实时开始/关闭、暂停/继续播放；                                  
> 7. 视频解码、线程控制、显示各部分功能分离，低耦合度。                               
> 8. 采用最新的5.1.2版本ffmpeg库进行开发，超详细注释信息，将所有踩过的坑、解决办法、注意事项都得很写清楚。
> 

* 下图中使用OpenGL显示YUV图像CPU占用率是使用QPainter显示的<mark>1/3左右</mark>，由于我使用的是非常老的笔记本的集显测试，所以GPU占用率比较高。

![image-20221017232820037](FFmpegDemo.assets/image-20221017232820037.png)



### 1.4 VideoPlayHW

> 1. 使用ffmpeg音视频库【硬解码】实现的视频播放器，采用GPU解码， 大幅降低对CPU的暂用率；
> 2. 支持打开本地视频文件（如mp4、mov、avi等）、网络视频流（rtsp、rtmp、http等）；
> 3. 支持视频匀速播放；
> 4. 采用QPainter进行显示，支持自适应窗口缩放；
> 5. 视频播放支持实时开始/关闭、暂停/继续播放；
> 6. 视频解码、线程控制、显示各部分功能分离，低耦合度。
> 7. 采用最新的5.1.2版本ffmpeg库进行开发，超详细注释信息，将所有踩过的坑、解决办法、注意事项都得很写清楚；
> 8. 展示了9路视频播放。

* 由于测试的电脑硬件性能过低，所以看起来效果不是很明显

![VideoPlayHW-tuya](FFmpegDemo.assets/VideoPlayHW-tuya.png)



### 1.5 VideoPlayHWGL

> 1. 使用ffmpeg音视频库【软/硬解码】实现的视频播放器；                                           
> 2. 支持打开本地视频文件（如mp4、mov、avi等）、网络视频流（rtsp、rtmp、http等）；                    
> 3. 支持视频匀速播放；                                                            
> 4. 采用【OpenGL显示YUV、NV12】图像，支持自适应窗口缩放，支持使用QOpenGLWidget、QOpenGLWindow显示；  
> 5. 将YUV/NV12转RGB的步骤由CPU转换改为使用GPU转换，降低CPU占用率；                            
> 6. 使用<mark>av_hwframe_map</mark>替代<mark>av_hwframe_transfer_data</mark>，可将【耗时降低1/3】；              
> 7. 视频播放支持实时开始/关闭、暂停/继续播放；                                               
> 8. 视频解码、线程控制、显示各部分功能分离，低耦合度。                                            
> 9. 采用最新的【5.1.2版本】ffmpeg库进行开发，超详细注释信息，将所有踩过的坑、解决办法、注意事项都得很写清楚。           

* 使用GPU解码 + OpenGL绘制大大降低了CPU占用率

![image-20221020192642223](FFmpegDemo.assets/image-20221020192642223.png)



### 1.6 AVIOReading

> 1. 将一个视频文件中说有数据读取到buf中；
> 2. 为AVIOContext创建一个回调函数；
> 3. 创建一个长度为4096内存avio_buf用于从buf中读取数据；
> 4. 使用回调函数完成数据的读取。

* 数据读取示例如下

![AVIOReading-tuya](FFmpegDemo.assets/AVIOReading-tuya.gif)

![image-20221024094954935](FFmpegDemo.assets/image-20221024094954935.png)
