# EReport

> 这是一个参考了MPQ C++ SDK而开发的事件转发插件，配合MPQ框架自身的转发接口，即可用于对接开发全双工通信的跨语言插件

注：本插件仅在MPQ当前免费版测试通过，稳定性也并不能够保证...

## 废话时间

这个插件诞生的契机是因为MPQ框架自带的转发接口功能不是很好用，尤其是我不明白在使用HTTP1.1协议的情况下，怎么去实现全双工通信？

其实我觉得有点神奇之处就在于：当开启了转发接口时，MPQ框架既能充当服务端又能充当客户端，既能转发事件到指定地址，也能接受这一地址的请求。

如果有人能告知我这是什么原理，感激不尽！

## 快速开始

- 下载[MyPCQQ](https://f.mypcqq.cc/thread-2327.htm)
- 从[Release界面](https://github.com/LonelySteve/EReport/releases)下载最新版本的EReport动态链接库
- 把下载好的EReport动态链接库移动到MPQ的Plugin目录下
- 启动MPQ
- 切换选项卡至“插件”
- 右键“EReport”插件启用

## 开发者须知

本项目使用VS2017构建，请使用VS2017以上的工具链构建

请重新配置项目属性，包括但不限于：

- 附加包含目录：
  - rapidjson
  - libcurl
- 附加库目录：
  - libcurl

如果上述库的目录路径重新设置正确后仍然无法正确构建的话，请提交issues

## 依赖

- [cpp-base64](https://github.com/ReneNyffenegger/cpp-base64)
- [rapidjson](https://github.com/Tencent/rapidjson)
- [libcurl](https://github.com/curl/curl)
