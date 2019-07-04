# yyl-bridge

yyl-bridge为一个c语言编写的sock5代理工具，需要同时部署客户端和服务端。

本项目仅作研究linux底层网络通信学习使用。

### 环境

系统：linux或mac

### 使用方式

##### 1. 构建编译

```bash
git clone https://github.com/yunyuliu/yyl-bridge.git
cd yyl-bridge
cmake .
make
```

##### 2. 使用

**客户端**

```bash
./yyl-bridge -P [本地监听端口] -c -h [服务器地址] -p [服务器端口]
```

**服务端**

```bash
./yyl-bridge -P [本地监听端口] -s
```

### 版本信息

##### v1.0

初版，仅支持不设密码的sock5协议，仅支持TCP代理，客户端与服务端之间未做压缩/加密处理。只具备初步可用性。

### 协议

本项目使用[MIT协议](https://github.com/yunyuliu/yyl-bridge/blob/master/LICENSE)。