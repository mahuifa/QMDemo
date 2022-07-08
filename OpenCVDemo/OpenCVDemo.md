# Qt使用OpenCV进行视频、图像操作Demo

[toc]

---

## 1、说明

| 类名      | 功能                                             |
| --------- | ------------------------------------------------ |
| VideoPlay | Qt使用OpenCV读取摄像头视频、本地视频、网络视频流 |
|           |                                                  |




## 2、相关博客

* [CSDN](https://blog.csdn.net/qq_43627907/category_11818354.html?spm=1001.2014.3001.5482)

## 3、实现效果

### 1.1 VideoPlay

> 1. 启动时获取所有可用摄像头名称；
> 2. 支持使用opencv打开本地摄像头，读取视频并显示；
> 3. 由于opencv包含ffmpeg，所以支持打开多种格式的本地视频文件并显示；
> 4. 支持打开多种协议的网络视频流并显示；
> 5. 支持传入显示QImage、QPixmap图片进行显示；
> 6. 支持图片的自适应窗口缩放，并保持纵横比。

![VideoPlay1-tuya](OpenCVDemo.assets/VideoPlay1-tuya.gif)