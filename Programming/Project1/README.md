## Project1

#### 完成情况

#### 文件架构

以下为文件夹架构，`src` 文件夹下为 `cpp` 源文件，`include` 文件夹下为 `hpp` 头文件，
`build` 文件夹下为可执行文件及编译产生的中间文件，`doc` 文件夹下为 `tex` 和 `pdf` 文件：

```
Project ──
   	|──doc
    |   |──figure
    |   |   └──*.pdf
    |   |──*.tex
    |   └──makeflie 
    |──build
    |   └──*
    |──include
    |   └──*.hpp
   	|──src
    |   └──*.cpp
	|──makefile
    |──template.json
	└──README.md
```

#### 环境配置

- 本次作业在 wsl 下完成编写和调试。

- `jsoncpp`
```
# 从官网下载源代码
git clone https://github.com/open-source-parsers/jsoncpp.git
cd jsoncpp
# 创建 build 文件用于存放编译产生的文件
mkdir -p build
cd build
# 用 cmake 编译
cmake -DCMAKE_BUILD_TYPE=release -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ..
# 用 make 编译
make
# 将头文件和库添加到 /local/include 和 /local/lib
sudo make install
```

- `lapack`
```
# 从官网下载源代码
wget https://github.com/Reference-LAPACK/lapack/archive/refs/tags/v3.12.0.tar.gz
# 解压
tar -zxvf v3.12.0.tar.gz
cd lapack-3.12.0
# make.inc 文件被包括在 lapack-3.12.0/Makefile 中
cp make.inc.example make.inc
# 用 make 编译
make -j8
# 到 LAPACKE 文件夹中编译
cd LAPACKE
make -j8
# 将头文件添加到 /local/include 和 /local/lib
sudo cp *.h /usr/local/include
# 将位于 lapack-3.12.0 文件夹下的库添加到 /local/include 和 /local/lib
sudo cp *.a /usr/local/lib 
```

#### 测试方法