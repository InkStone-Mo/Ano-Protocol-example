| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# 基于ESP32 串口的匿名协议示例

> 本示例使用的IO是 GPIO0 (TX) , GPIO1 (RX)

## 如何编译本示例？
1. 首先下载官方的IDF源码
> git clone https://github.com/espressif/esp-idf.git --recurse-submodules

2. 到源码中运行脚本自动配置编译环境
cd esp-idf
. export.sh

3. 回到本目录，然后编译即可
idf.py build
