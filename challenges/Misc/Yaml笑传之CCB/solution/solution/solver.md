## 答案

运行`./encode.sh`即可

## 背景

出题思路来源于[这个博客](https://hitchdev.com/strictyaml/)。一开始偶然接触到了yaml的[“挪威问题”](https://hitchdev.com/strictyaml/why/implicit-typing-removed/)(Norway problem)，想着以这个为主题出一个题，但没有好的思路。顺着“挪威问题”我找到了上述博客，作者在项目StrictYAML中把yaml本身的问题以及pyyaml不足的地方全都批判了一番，其中有一点便是用pyyaml可以生成任意python对象，同样的问题在Ruby甚至造成[Ruby on Rail的一个CVE](https://codeclimate.com/blog/rails-remote-code-execution-vulnerability-explained)(CVE-2013-0156)。因此我便尝试在Python中引入类似的漏洞，发现很容易在一个用pyyaml解析用户输入的程序中实现RCE,于是就稍微改成了一个Misc题

## 解析

如[博客](https://hitchdev.com/strictyaml/why/not-parse-direct-representations-of-python-objects/)以及[官方文档](https://pyyaml.org/wiki/PyYAMLDocumentation)所说，pyyaml在解析yaml文本/文件的时候可以用一种方式把yaml文本直接解析为Python对象，甚至能做到任意Python代码执行（摘自官方文档）：

```txt
Objects

Any pickleable object can be serialized using the !!python/object tag:

!!python/object:module.Class { attribute: value, ... }

In order to support the pickle protocol, two additional forms of the !!python/object tag are provided:

!!python/object/new:module.Class
args: [argument, ...]
kwds: {key: value, ...}
state: ...
listitems: [item, ...]
dictitems: [key: value, ...]

!!python/object/apply:module.function
args: [argument, ...]
kwds: {key: value, ...}
state: ...
listitems: [item, ...]
dictitems: [key: value, ...]

If only the args field is non-empty, the above records can be shortened:

!!python/object/new:module.Class [argument, ...]

!!python/object/apply:module.function [argument, ...]
```

于是构造这样的yaml,在解析阶段就能够执行：

```yaml
!!python/object/new:os.system
args: ["bash"]
```

该特性可以调用python中任意模块的函数和类构造，功能很强大

答案用的是os的system模块，解法不止一种
