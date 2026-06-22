## Homework 8

#### 文件架构

以下为文件夹架构，`src` 文件夹下为 `cpp` 源文件，`include` 文件夹下为 `hpp` 头文件，
`build` 文件夹下为可执行文件及编译产生的中间文件，`doc` 文件夹下为 `tex` 和 `pdf` 文件：

```
Project ──
    |──figure
    |   └──*
    |──python
    |   └──*.py
	|──makefile
    |──main.tex
    |──citation.bib
	└──README.md
```

#### 环境配置

- 本次作业在 wsl 下完成编写和调试。

#### 测试方法

- 在根目录下使用 `make` 可以在根文件夹下编译产生报告。

- 修改 `python` 目录下的 `Program14.4.X.py` 文件中参数
（例如 `r`，`methodLabel`等等），并使用 `make`，查看 `figure`
文件夹下的 `Program14.4.X.pdf` ，来测试不同输入下的程序运行结果。