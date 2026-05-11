# BITs2CTF 2025

第一届北京理工大学 CTF 新生赛（BITs2CTF 2025）的官方归档仓库。

- **比赛时间**：2025-11-22 ~ 2025-11-23（UTC+8 09:00 ~ 次日 09:00）
- **平台**：[GZCTF](https://github.com/GZTimeWalker/GZCTF)
- **分组**：学生组、大一新生组

## 目录结构

```
BITs2CTF-2025/
├── challenges/                # 全部题目（按方向分类）
│   ├── Misc/    37 题（含 30 道选择题）
│   ├── Crypto/   8 题
│   ├── PWN/      8 题
│   ├── Web/      8 题
│   ├── Reverse/  8 题
│   └── PPC/      4 题
│
├── Writeups/                  # 所有提交 WriteUp 的战队 PDF（共 70 份），按战队名命名
└── scoreboard.json            # 完赛后的官方排行榜（GZCTF API 原始返回）
```

每道题目目录的标准结构：

```
<challenge>/
├── description.md             # 题目描述（来自平台 Content 字段）
├── 题目设计说明书.docx         # 出题人写的设计说明（部分题目缺失）
├── src/                       # 题目源码（Dockerfile / 服务端代码 / 等）
├── solution/                  # 题解（出题人的 WP / 解题脚本）
├── attachment/                # 选手下载附件（如有）
├── docker_image/              # 题目靶机镜像（见下方说明，仅含 README）
└── _extras/                   # 出题人留下的辅助资料（图片、笔记等）
```

并非所有题目都包含全部子目录——比如选择题只有 `description.md`，而部分题目的源码/题解没有归档。

## Docker 镜像

由于 GitHub 单文件 100 MB 上限，全部 26 个题目靶机镜像作为 [Release](https://github.com/BITs2Sys/BITs2CTF-2025/releases) 资产单独存放。
每个有靶机的题目目录下的 `docker_image/README.md` 给出了对应镜像的下载链接和加载命令：

```bash
docker load -i "<image-name>.tar.gz"
```

## 排行榜

`scoreboard.json` 是 GZCTF `/api/game/4/scoreboard` 的完整 JSON 输出，包含所有分组的实时积分曲线、最终排名、做题情况、一血/二血/三血加成（`bloodBonus = 31477770` 解码为 1st blood +3%、2nd +2%、3rd +1%）。

## 技术栈速查

| 类别 | 题目（实际比赛中） |
|---|---|
| 选择题 | Choice_01 ~ Choice_30 |
| Crypto | Are you crazy / babyRSA / changeable / easyAES / play_and_show_yourself / remaining thing / SCA01-ECC / SCA02-RSA |
| PWN | 三剑齐出 / 和溢位？/ 幻想乡巴士调度系统 / 物理实验选课网站 / 签顺道 / 🥷写死你·内核原语 / 🥷吓我一跳我释放堆块 / 🥷我设了一个笼 |
| Web | 1zvm / CCTest / DeepSleep / babep1ckle / exit / http / noobUnser / u8c_deserialize |
| Reverse | ChaCha20 / Easy_maze / LLVM Pass / gore / math / rustre / 奶龙与小七之大战WebAssembly / 奶龙与小七之真假奶龙 |
| PPC | A.故障机器人 / B.QwQwQwQ / C.争风吃醋 / D.梦符「阴阳结界」 |
| Misc | 签到 / 我找不到比赛平台的域名了 / 玄机在哪 / 听说你学过工程制图？/ GCC偷走了重要的函数！/ Rust，启动！/ Yaml笑传之CCB |

## 致谢

感谢全部出题人、运维与志愿者，以及参与本届新生赛的所有同学。
