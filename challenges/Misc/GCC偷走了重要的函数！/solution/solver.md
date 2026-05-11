## 答案

运行`./compress.py`即可

## 背景

出题思路来自于IOCCC的[这个作品](https://www.ioccc.org/1984/mullender/index.html)代码和该[博客](https://jroweboy.github.io/c/asm/2015/01/26/when-is-main-not-a-function.html)，很早（大概大一上）就觉得很有意思，当时想复现一下没有成功。后来深入学习了C语言相关内容，明白了代码的原理，但还是无法在现在的环境复现成功。去年在浏览GCC官方文档的时候注意到了[这个attribute](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-section-function-attribute)，就有了想法，写了个demo出来竟然能够成功编译并运行，现在正好拿来出一道Misc

## 解析

### 前置知识

题目通过用treesitter解析AST来判断有没有使用函数，如果有函数定义或声明，就禁止通过检查，因此我们要尝试在没有函数的情况下写出可以编译并运行的C源代码。

在下一步之前，我们首先来看一下Linux系统下C语言编译和链接相关的知识

在使用gcc时，我们一般是直接`gcc xxx.c -o xxx.out`得到可执行文件。现在我们把它拆成两步来：

1. `gcc -c xxx.c -o xxx.o`
2. `gcc xxx.o -o xxx.out`

第一步生成目标文件，第二步是将目标文件链接成可执行文件。

回想起我们在写C代码的时候，并没有引入`printf`,`scanf`这种函数的具体实现（可以去看头文件里边，仅仅给出了定义），而这些函数的具体实现则在标准库glibc中，ld链接器通过动态链接技术提供运行时支持，也就是第二步的主要工作

同时，在glibc中还规定了函数的入口：符号main

为什么说是“符号”而不是“函数”，是因为链接器并不知道什么是函数，它只通过“符号”来判断该如何链接。除了函数外，全局变量，加了static的局部变量等都会被处理作符号，在链接时会一视同仁，并不管是否正确地将“函数”还是错误地将某个同名“全局变量”链接到了一个函数调用上。

所以，我们可以通过这样的方法来绕过限制：将全局变量main链接到函数的入口处，让它作为函数去执行。

事实上，计算机并无法分辨清“指令”和“数据”，在计算机眼里都是一堆二进制数据，我们完全可以让它把“指令”当作“数据”处理，或者把“数据”当作“指令”执行

所以思路就很明显：把能输出"Hello World\n"的指令当作数据输出，定义到全局变量里去，再让计算机把该数据当作指令执行

### 绕过权限限制

通过python脚本可以生成机器码，于是我们有了这个demo:

```c
unsigned char  main[] = {
    0x48, 0x31, 0xc0, 0x48, 0xff, 0xc0, 0x48, 0xc7, 0xc1, 0x72, 0x6c,
    0x64, 0x0a, 0x51, 0x48, 0xb9, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
    0x57, 0x6f, 0x51, 0x48, 0x31, 0xff, 0x48, 0xff, 0xc7, 0x48, 0x89,
    0xe6, 0x48, 0xc7, 0xc2, 0x0c, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x48,
    0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, 0x48, 0x31, 0xff, 0x0f, 0x05};
```

可以尝试，该代码是能够正常通过编译的，但是执行会报错，这是因为虽然计算机想要执行该数据，但是全局变量并没有执行的权限

事实上，Linux系统下有严格的执行权限验证机制，定义的全局变量会被放到一个可读可写不可执行的段里边（bss段/data段），而函数则位于一个可读可执行不可写的段里边（text段），当我们尝试执行main时，会因为没有权限而报错

阅读GCC官方手册，可以[注意到](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-section-function-attribute)这个attribute: `section ("section-name")`

可以看出，该attribute可以允许我们自定义一个变量处于的代码段，我们甚至可以直接将一个变量定义到text段里边，使其获得执行权限：

```c
unsigned char __attribute__((section(".text"))) main[] = {
    0x48, 0x31, 0xc0, 0x48, 0xff, 0xc0, 0x48, 0xc7, 0xc1, 0x72, 0x6c,
    0x64, 0x0a, 0x51, 0x48, 0xb9, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
    0x57, 0x6f, 0x51, 0x48, 0x31, 0xff, 0x48, 0xff, 0xc7, 0x48, 0x89,
    0xe6, 0x48, 0xc7, 0xc2, 0x0c, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x48,
    0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, 0x48, 0x31, 0xff, 0x0f, 0x05};
```

该代码便是最终的答案，可以看出完全可以压缩成一行，拥有执行权限，并能够输出"Hello World\n"

## 其他

我一直想写出一个不依赖头文件，与IOCCC代码类似但能成功编译执行的程序，最开始通过`mprotect`来设置权限，代码为：

```c
#include <sys/mman.h>
int
main()
{
 char *hamood =
  "WTYH39Yj3TYfi9WmWZj8TYfi9JBWAXjKTYfi9kCWAYjCTYfi93iWAZjKTYfi9GD0T820T830"
  "T840T850T8D0T8E0t8I0t8K0t8N0T8T0T8U0T8V0T8W0T8X0t8Z0T8e0T8f0T8g0T8h0t8k0"
  "T8l0T8mRAPZ0t800t810t8L0t8O0t8Q0t8R0T8S0t8a0t8cZRARZ0t880t8dZjaTYfi97l0t"
  "800t830t8S0t8T0T8URAPZ0t8QZRARZ0t81ZHGCEUUUH1HH7HHqHamoodjUQH17H7GHAfHGB"
  "8UUUZPH7KHK3UuTHGHiUUUH17ZP";
 mprotect((void *)((unsigned long long)hamood & 0xfffffffffffff000),
   0x1000, 7);
 ((void (*)(void))hamood)();
 return 0;
}
```

但我认为不够优雅，需要引入其他函数，而且一眼就能看明白运行原理

后来意外地在GCC官方手册发现了解决方法，修改后发现能直接编译运行

最终代码定稿与去年九月，demo为:

```c
unsigned char __attribute__((section(".text"))) main[] = {
 'H',  0xc7, 0xc3, 0x10, 0x00, 0x00, 0x00, 'H', '1',  0xc0, 'H',
 0xff, 0xc0, 'H',  0xb9, 'H',  'a',  'm',  'o', 'o',  'd',  '?',
 0x0,  'Q',  'H',  '1', 0xff, 'H',  0xff, 0xc7, 'H',  0x89, 0xe6,
 'H',  0xc7, 0xc2, 0x08, 0x00, 0x00, 0x00, 0x0f, 0x05, 'H',  0xff,
 0xcb, 'H',  0x83, 0xfb, 0x00, 'u',  0xd4, 'H', 0xc7, 0xc0, '<',
 0x00, 0x00, 0x00, 'H', '1',  0xff, 0x0f, 0x05
};
```

> 在浏览IOCCC其他代码时发现了本题的一个恶性DOS漏洞（直接把我的电脑弄蓝屏了），于是将其ban掉了，同时也是为了防止非预期
