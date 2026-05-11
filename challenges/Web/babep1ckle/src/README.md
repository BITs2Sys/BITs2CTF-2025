# Web - babep1ckle

lamaper

**tags**: SSTI ; Flask ; Session伪造 ; pickle反序列化

## flag设置

本题预期通过用户名展示漏洞SSTI获取位于session中的SecretKey，从而伪造JWT中身份为admin开启反序列化入口，通过pickle反序列化获取/flag

## 题目说明

本题意图考察Python Jinja2 SSTI、Session伪造、pickle反序列化。

## 环境说明

本题采用 `Python 3.10 + Flask` 的基础环境，默认暴露端口位于8888

## 如何使用

本题在dockercompose中启用了volumes以方便调试，正式环境中可以删除。

使用 `./docker/docker-compose.yml` 内的 `docker-compose` 文件实现一键启动测试容器

```shell
cd ./docker
docker compose up -d --build
```