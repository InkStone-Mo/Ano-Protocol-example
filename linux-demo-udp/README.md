## 请耐心阅读本文档，已做出详细的使用解释  

### 说明  
本代码将命令功能、参数功能、升级功能以及其它功能拆分成多个文件便于阅读和理解代码
编译后会生成对应的可执行文件，比如command_anoUdp、parameter_anoUdp等

### 编译  
在linux环境中，执行  
> mkdir build  
> cd build  
> cmake ..&&make -j8  

### 编译后使用  
编译后在文件中生成anoUdp可执行文件  
![](/doc/build_run.png)  
直接执行，然后打开上位机填入本机ip以及预设好的12345端口即可正常使用  
打开连接后，点击读取设备信息就能获取到代码中注册的设备信息
![](/doc/ano_show.png)  
