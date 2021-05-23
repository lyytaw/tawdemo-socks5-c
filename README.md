# tawdemo-socks5-c

tawdemo-socks5-c为一个C语言编写的sock5代理工具DEMO，需要同时部署客户端和服务端。

本项目仅作研究linux底层网络通信学习使用。不具备生产特性，请勿用于生产环境。

ps: 个人并不是专业写C语言的，使用C语言只是为了加深对网络协议底层处理的了解，因此项目代码风格非常糟糕，请勿随意模仿，还请见谅

### 环境

系统：linux或mac

### 使用方式

##### 1. 构建编译

```bash
git clone tawdemo-socks5-c
cd tawdemo-socks5-c
cmake .
make
```

##### 2. 使用

**客户端**

```bash
./tawdemo-socks5-c -P [本地监听端口] -c -h [服务器地址] -p [服务器端口]
```

**服务端**

```bash
./tawdemo-socks5-c -P [本地监听端口] -s
```

### 版本信息

##### v1.0

初版，仅支持不设密码的sock5协议，仅支持TCP代理，客户端与服务端之间未做压缩/加密处理。只具备初步可用性。

### 协议

本项目使用[MIT协议](https://github.com/lyytaw/tawdemo-socks5-c/blob/master/LICENSE)。
