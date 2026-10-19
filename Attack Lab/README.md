# Attack Lab
## 实验简介
本实验通过缓冲区溢出攻击两个含有漏洞的程序，其中：
- ctarget 目标文件没有使用栈随机化和栈不可执行，可以使用代码注入攻击
- rtarget 提供了code farm,可对其进行返回导向编程攻击
- 每个target中都包含如下函数可供缓冲区溢出攻击：
```assembly
00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 ac 03 00 00       	call   401b60 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	ret
  4017be:	90                   	nop
  4017bf:	90                   	nop
```
- 执行call指令时，等价于执行push %rip，jmp <目标函数>,因此每个函数栈帧的上方8字节为ret指令将会使用的地址，我们要对这个地址进行修改
## 第一阶段
### 第一题
任务是call touch1，由于getbuf分配了0x28字节的栈空间，我们只需构造一个长为40字节的任意字符串，再加上touch1函数的地址即可（注意大小端序）

### 第二题
任务是call touch2，使用cookie作为参数

回忆：Linux-x86函数调用约定：前六个参数分别存在%rdi,%rsi,%rdx,%rcx,%r8,%r9中

因此我们需要构造一段程序，包含以下指令：
```assembly
movabs $0x59b997fa,%rdi
push 0x4017ec # touch2
ret
```
构造字符串，需要用gdb读出栈地址%rsp（这一阶段栈不随机化且可执行），前面放这些指令的字节码，第41个字符开始是栈指针即可

### 第三题
任务是call touch3，传入一个字符串作参数，需要与目标字符串cookie匹配


