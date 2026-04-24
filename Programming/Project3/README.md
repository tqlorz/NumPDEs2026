## Project1

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
    |──python
    |   └──*.py
    |──output
    |   |──data
    |   |   └──*.txt
    |   |──figure
    |   |   └──*.pdf
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

- 在根目录下使用 `make story` 可以在`doc`文件夹下编译产生报告。

- 修改根目录下的 `template.json` 文件，并使用 `make run`，查看 `output/figure`
文件夹下的 `result.pdf` ，来测试不同输入下的程序运行结果。

#### json 文件格式

```
{
    "Method_Label": 1,     # LMM 方法：1—Adams-Bashforth, 1—Adams-Moulton, 3—BDFs
    "p": 2,                # LMM 阶数
    "Time_Step": 0.01,     # 时间步长
    "Case_Label": 1,       # 测试案例：1—case1, 2—case2
    "Case_Description": {
        "Case_1" : {
            "mu": 0.012277471,
            "Total_Time": 17.06521656015796,
            "Initial_Value": [0.944, 0, 0,
                              0, -2.0015851063790825224, 0]
        },
        "Case_2" : {
            "mu": 0.012277471,
            "Total_Time": 19.140540691377,
            "Initial_Value": [0.879779227778, 0, 0,
                              0, -0.379677780949, 0]
        }
    },
    "Method_Name": ["Adams-Bashforth", 
                    "Adams-Moulton", 
                    "BDFs"]
}
```