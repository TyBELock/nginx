# Nginx 源码目录结构说明

> **版本**: nginx/1.28.1
> **更新时间**: 2025-12-29
> **Commit**: 95078974cfe998ec4095ac2c504c5c92bbc9a326

本文档详细介绍了 Nginx 源码的目录结构、各模块功能以及编译输出文件。

## 目录概览

```
nginx/
├── auto/           # 构建系统和配置脚本
├── conf/           # 配置文件示例
├── contrib/        # 第三方贡献的工具和脚本
├── docs/           # 文档资源
├── misc/           # 杂项工具和脚本
├── objs/           # 编译输出目录（自动生成）
├── src/            # 核心源代码
└── build.log       # 编译日志
```

## 核心目录详解

### 1. `src/` - 核心源代码目录

这是 Nginx 最主要的目录，包含所有的源代码文件（134 个 .c 文件，258 个 .h 头文件）。

```
src/
├── core/       # 核心模块 (920K)
├── event/      # 事件驱动机制 (1.1M)
├── http/       # HTTP 服务器模块 (3.4M)
├── mail/       # 邮件代理模块 (316K)
├── stream/     # TCP/UDP 代理模块 (688K)
├── os/         # 操作系统抽象层 (680K)
└── misc/       # 杂项工具 (12K)
```

#### 1.1 `src/core/` - 核心模块

**功能**: Nginx 的核心基础设施，提供基础数据结构和工具函数。

**主要文件**:
- `nginx.c` - 主程序入口，进程管理
- `ngx_conf_file.c` - 配置文件解析器
- `ngx_cycle.c` - 生命周期管理
- `ngx_connection.c` - 连接管理
- `ngx_array.c/h` - 动态数组实现
- `ngx_hash.c/h` - 哈希表实现
- `ngx_rbtree.c/h` - 红黑树实现
- `ngx_string.c/h` - 字符串处理
- `ngx_alloc.c/h` - 内存管理
- `ngx_log.c/h` - 日志系统
- `ngx_palloc.c/h` - 内存池管理

**核心概念**:
- **Cycle**: Nginx 的生命周期管理结构
- **Module**: 模块系统的基础
- **Configuration**: 配置解析和管理

#### 1.2 `src/event/` - 事件驱动机制

**功能**: 实现事件驱动架构，这是 Nginx 高性能的关键。

**主要文件**:
- `ngx_event.c/h` - 事件核心框架
- `ngx_event_timer.c` - 定时器实现
- `ngx_event_accept.c` - 连接接受处理
- `ngx_event_connect.c` - 连接建立
- `ngx_event_pipe.c` - 管道处理
- `ngx_event_openssl.c` - OpenSSL 事件集成
- `modules/` - 事件模块（epoll, kqueue, etc.）
- `quic/` - HTTP/3 和 QUIC 协议实现

**子目录**:
- `modules/ngx_epoll_module.c` - Linux epoll 实现
- `modules/ngx_kqueue_module.c` - FreeBSD kqueue 实现
- `modules/ngx_poll_module.c` - poll 机制
- `modules/ngx_select_module.c` - select 机制
- `quic/` - QUIC 协议完整实现（18个文件）

**核心特性**:
- 异步 I/O 事件处理
- 高效的定时器机制
- 跨平台事件抽象

#### 1.3 `src/http/` - HTTP 服务器模块

**功能**: HTTP 协议的完整实现，包括 HTTP/1.x, HTTP/2 和 HTTP/3。

**主要文件**:
- `ngx_http.c` - HTTP 模块主入口
- `ngx_http_core_module.c` - HTTP 核心功能
- `ngx_http_request.c/h` - HTTP 请求处理
- `ngx_http_parse.c` - HTTP 协议解析
- `ngx_http_upstream.c` - 反向代理和负载均衡
- `ngx_http_file_cache.c` - 文件缓存
- `ngx_http_variables.c` - 变量系统

**子模块** (`modules/`):
- `ngx_http_*_module.c` - 各种 HTTP 功能模块
  - SSL/TLS 支持
  - Gzip 压缩
  - 日志记录
  - 访问控制
  - 认证等

**协议版本**:
- `v2/` - HTTP/2 实现（9个文件）
- `v3/` - HTTP/3 实现（15个文件）

**核心功能**:
- 虚拟主机
- 访问控制
- 内容压缩
- SSL/TLS 加密
- 负载均衡
- 缓存系统

#### 1.4 `src/mail/` - 邮件代理模块

**功能**: 实现 IMAP、POP3 和 SMTP 协议的代理功能。

**主要文件**:
- 邮件协议解析
- 邮件服务器代理
- 认证支持

**适用场景**:
- 邮件服务器代理
- 邮件协议负载均衡

#### 1.5 `src/stream/` - TCP/UDP 代理模块

**功能**: 通用 TCP/UDP 代理，也称为 "stream" 模块。

**主要文件**:
- TCP/UDP 代理核心逻辑
- 流量控制
- 访问控制
- SSL 代理支持

**适用场景**:
- 数据库代理（MySQL, PostgreSQL 等）
- DNS 代理
- 通用 TCP 服务负载均衡

#### 1.6 `src/os/` - 操作系统抽象层

**功能**: 提供跨平台的操作系统接口抽象。

**子目录**:
- `unix/` - Unix-like 系统实现
  - `ngx_linux_init.c` - Linux 特定初始化
  - `ngx_posix_init.c` - POSIX 标准实现
  - `ngx_process.c` - 进程管理
  - `ngx_socket.c` - Socket 封装
  - `ngx_files.c` - 文件操作

**核心功能**:
- 线程管理
- 进程管理
- 文件 I/O
- 网络 I/O
- 共享内存

#### 1.7 `src/misc/` - 杂项工具

包含一些辅助工具和实用函数。

### 2. `auto/` - 构建系统目录

包含构建 Nginx 所需的所有配置脚本。

```
auto/
├── cc/          # 编译器相关配置
├── lib/         # 第三方库检测
├── os/          # 操作系统特性检测
├── types/       # 数据类型检测
├── configure    # 配置脚本主入口
├── options      # 配置选项定义
├── sources      # 源文件列表
└── make         # Makefile 生成
```

**功能**:
- 检测系统特性
- 配置编译选项
- 生成 Makefile
- 检测依赖库（PCRE, OpenSSL, zlib）

**重要文件**:
- `auto/configure` - 配置脚本入口
- `auto/options` - 所有 configure 参数定义
- `auto/sources` - 定义各模块的源文件列表

### 3. `conf/` - 配置文件目录

包含 Nginx 的默认配置文件示例。

**主要文件**:
- `nginx.conf` - 主配置文件示例
- `fastcgi.conf` - FastCGI 配置
- `mime.types` - MIME 类型定义

### 4. `objs/` - 编译输出目录

编译过程中自动生成的目录，包含中间文件和最终产物。

```
objs/
├── ngx_auto_config.h      # 自动生成的配置头文件
├── ngx_auto_headers.h     # 头文件定义
├── ngx_modules.c          # 模块列表
├── Makefile               # 编译 Makefile
└── src/                   # 编译生成的 .o 目标文件
```

**编译产物**:
- `objs/nginx` - 最终的可执行二进制文件（当前编译：6.9M）
- `*.o` - 目标文件
- `*.err` - 配置错误日志

### 5. `contrib/` - 第三方贡献工具

包含社区贡献的有用工具和脚本。

**子目录**:
- `vim/` - Vim 编辑器语法高亮和缩进规则
- `unicode2nginx/` - Unicode 转换工具

### 6. `docs/` - 文档目录

包含各种格式的文档资源。

```
docs/
├── dtd/        # 文档类型定义
├── html/       # HTML 格式文档
├── man/        # Man page 文档
├── xml/        # XML 源文档
├── xsls/       # XSLT 样式表
└── xslt/       # XSLT 转换工具
```

### 7. `misc/` - 杂项工具目录

包含一些有用的辅助脚本和工具。

**内容**:
- 测试脚本
- 辅助工具
- 示例代码

## 编译相关文件

### 1. `Makefile` (根目录)

简化的 Makefile，调用 `objs/Makefile` 进行实际编译。

```makefile
default:	build

build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage
```

### 2. `compile_commands.json`

**用途**: 用于代码分析工具（LSP, clangd 等）

**大小**: 132KB
**条目数**: 183 个编译条目
**生成工具**: Bear

**格式**:
```json
{
  "arguments": [编译命令数组],
  "directory": "工作目录",
  "file": "源文件路径",
  "output": "输出文件路径"
}
```

### 3. `compile.json`

**用途**: 编译配置信息记录

**内容**:
- Nginx 版本
- Git 提交信息
- 编译器版本
- 依赖库版本
- 启用的模块
- 系统信息

### 4. `build.log`

**用途**: 编译过程日志

## 根目录文件说明

| 文件 | 说明 |
|------|------|
| `README.md` | 项目介绍和快速开始指南 |
| `LICENSE` | 2-clause BSD-like 许可证 |
| `CHANGES` | 版本变更日志 |
| `CODE_OF_CONDUCT.md` | 贡献者行为准则 |
| `CONTRIBUTING.md` | 贡献指南 |
| `SECURITY.md` | 安全漏洞报告流程 |

## 编译配置

### 当前编译配置

```bash
./auto/configure --prefix=/usr/local/nginx \
  --with-http_ssl_module \
  --with-http_realip_module \
  --with-http_gzip_static_module \
  --with-http_stub_status_module \
  --with-http_v2_module \
  --with-http_v3_module \
  --with-stream \
  --with-stream_ssl_module
```

### 编译输出

- **二进制文件**: `objs/nginx` (6.9M)
- **编译器**: gcc 11.4.0
- **依赖库**:
  - PCRE2 (系统库)
  - OpenSSL 3.0.2
  - zlib (系统库)

## 代码统计

### 源代码分布

```
总源文件数:  134 个 .c 文件
总头文件数:  258 个 .h 文件
总代码量:    约 7.1M (src/ 目录)
```

### 模块大小分布

| 模块 | 大小 | 占比 | 说明 |
|------|------|------|------|
| HTTP | 3.4M | 48% | HTTP 协议实现 |
| Event | 1.1M | 15% | 事件驱动机制 |
| Core | 920K | 13% | 核心基础设施 |
| Stream | 688K | 10% | TCP/UDP 代理 |
| OS | 680K | 10% | 操作系统抽象 |
| Mail | 316K | 4% | 邮件代理 |
| Misc | 12K | <1% | 杂项工具 |

## 开发相关

### 如何使用 compile_commands.json

这个文件可以被以下工具使用：

1. **Vim/Neovim**:
   - coc.nvim (clangd)
   - vim-lsp

2. **VS Code**:
   - C/C++ Extension
   - clangd

3. **代码分析**:
   - clang-tidy
   - cppcheck

4. **IDE**:
   - Qt Creator
   - CLion

### 编译流程

```bash
# 1. 配置
./auto/configure [选项]

# 2. 编译
make -j$(nproc)

# 3. 生成编译数据库（可选）
bear -- make -j$(nproc)

# 4. 安装
make install
```

## 核心概念

### 模块化架构

Nginx 采用高度模块化的设计：
- **核心模块** (`src/core/`) - 提供基础设施
- **事件模块** (`src/event/`) - 事件驱动
- **HTTP 模块** (`src/http/`) - HTTP 功能
- **Mail 模块** (`src/mail/`) - 邮件代理
- **Stream 模块** (`src/stream/`) - TCP/UDP 代理

### 进程模型

- **Master 进程**: 管理 worker 进程，读取配置
- **Worker 进程**: 处理实际请求
- **Cache 进程**: 负责缓存管理
- **共享内存**: 进程间通信

### 事件驱动

- 基于 epoll/kqueue/IOCP 等事件机制
- 异步非阻塞 I/O
- 高效的定时器实现

## 参考资源

- [官方文档](https://nginx.org/en/docs/)
- [开发指南](https://nginx.org/en/docs/dev/development_guide.html)
- [模块开发](https://nginx.org/en/docs/dev/development_guide.html)

## 版本信息

```
nginx version: nginx/1.28.1
built by gcc 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04.2)
built with OpenSSL 3.0.2 15 Mar 2022
TLS SNI support enabled
```

---

**文档生成时间**: 2025-12-29
**Git Commit**: 95078974cfe998ec4095ac2c504c5c92bbc9a326
**分支**: stable-1.28
