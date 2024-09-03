# QTScrcpy

**简单的需求**

1、截屏（可更改大小，并保存）

2、录屏，自定义录屏位置

3、录取声音（与录屏同步）

4、将录屏和声音进行合成，保存到本地

5、对视频进行传输，测试使用TCP连接，UDP传输数据的方式对数据进行传输

6、



# 1、封装adb命令

adb命令为外部的进程，qt中使用外部进程需要使用QtProcess。

继承QTProcess使用自己的AdbProcess

## 1.1 设置动态获取adb.exe文件的路径



## 1.2 使用到的命令

push : 将本地文件push到手机端（远端）的命令

remove : 将文件从手机上删除(shell rm)

reverse : 建立反向代理（将安卓端的套接字连接到PC端的端口上）

reverseRemove ：删除反向代理



# 2 server

启动命令：

```
// adb shell /data/local/tmp/scrcpy-srever.jar app_process / com.genymobile.scrcpy.Server maxsize bitrate false ""
// adb shell /data/local/tmp/scrcpy-srever.jar app_process / com.genymobile.scrcpy.Server 1080 800000 false ""
```

启动流程：

push、反向代理、启动服务；

在这个过程中可能会存在启动不成功的情况，需要清理的操作；----使用状态机编程方法

```
状态机编程方法
1、使用enum 创建枚举的状态过程
2、创建一个状态变量，表示要执行的状态过程
3、根据不同的状态进行实现

```

信号槽函数

```
connect(&m_workProcess, &AdbProcess::adbProcessResult, this, &server::onWorkProcessResult);
```

连接 `AdbProcess` 类的 `adbProcessResult` 信号到 `server` 类的 `onWorkProcessResult` 槽

**信号和槽的基本机制**

- **信号**：信号本质上是一个函数声明（没有实现），用于在对象之间传递事件。当信号被触发时（通过 `emit` 关键字），Qt 框架会自动调用与该信号连接的槽函数。
- **槽函数**：槽函数是一个普通的成员函数，但可以与信号连接。槽函数可以有任何返回类型（通常是 `void`），并且可以接受与信号相同的参数。

**触发机制**

当你在代码中调用类似以下内容时：

```
emit adbProcessResult(result);
```

所有连接到 `adbProcessResult` 信号的槽函数都会被立即调用。例如：

```
connect(&m_workProcess, &AdbProcess::adbProcessResult, this, &server::onWorkProcessResult);
```

当 `emit adbProcessResult(result)` 被执行时，`server` 对象的 `onWorkProcessResult` 槽函数会被调用，并接收到 `result` 参数。



Scrcpy

键盘连接win、蓝牙连接mac电脑实现了手机播放的内容，同步到电脑上；



视频传输的时候减少延时，直播的类型，自动降低码流，自动给包头打上堵塞状态包；







