# Shell Lab
## 项目简介
根据CSAPP 第八章和Shell Lab实现的tiny shell，具体实现细节详见tsh.c的代码与注释
## 实验环境
本实验主要代码在wsl环境下编写，应在linux系统环境下运行
## 文件结构
-tsh.c tiny shell 源代码，包含详细的注释

-tsh tiny shell 可执行文件
## 主要功能
- 支持前台和后台命令执行；

- 支持命令行末尾 & 将作业放入后台；

- 支持内置命令：

  - quit：退出 shell；

  - jobs：列出当前作业；

  - bg <PID|%JID>：将停止的作业切换为后台运行；

  - fg <PID|%JID>：将作业切换为前台运行并等待；

- 支持 Ctrl-C 向前台作业发送 SIGINT；

- 支持 Ctrl-Z 向前台作业发送 SIGTSTP；

- 通过 SIGCHLD 回收终止子进程，并处理停止/终止状态；

- 每个子进程使用独立进程组，避免后台作业收到终端的 SIGINT / SIGTSTP。
## 程序架构
### tsh.c 主要可以划分为以下几部分：

- 主程序与命令循环

  - main：安装信号处理器、初始化作业列表、循环读取并执行命令。

- 命令求值与解析

  - eval：shell 的核心求值函数；

  - parseline：解析命令行，构建 argv，判断是否后台运行。

- 内置命令

  - builtin_cmd：识别并执行 quit、jobs、bg、fg；

  - do_bgfg：实现 bg 和 fg 的具体逻辑。

- 前台等待

  - waitfg：等待前台作业不再是前台状态。

- 信号处理

  - sigchld_handler：处理子进程终止或停止；

  - sigint_handler：转发 SIGINT 给前台作业；

  - sigtstp_handler：转发 SIGTSTP 给前台作业；

  - sigquit_handler：用于测试时优雅退出 shell。

- 作业列表管理

  - clearjob、initjobs、addjob、deletejob；

  - fgpid、getjobpid、getjobjid、pid2jid、listjobs。

- 其他辅助函数

  - usage、unix_error、app_error、Signal 等。




