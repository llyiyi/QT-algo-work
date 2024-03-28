# QT-algo-work

本项目基于https://github.com/bbdzs/Sorting-Visualization-bbdzs。
一款基于QT-C++的可视化排序软件，实现了常用排序算法的可视化展示、时间统计、算法效率比较，并设计了“最快”的排序算法以及资源有限状态的外部排序算法。

<!-- PROJECT SHIELDS -->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]

<!-- PROJECT LOGO -->

## 目录

- [上手指南](#上手指南)
  - [开发前的配置要求](#开发前的配置要求)
  - [安装步骤](#安装步骤)
- [文件目录说明](#文件目录说明)
- [开发的架构](#开发的架构)
- [部署](#部署)
- [使用到的框架](#使用到的框架)
- [贡献者](#贡献者)
  - [如何参与开源项目](#如何参与开源项目)
- [版本控制](#版本控制)
- [作者](#作者)
- [鸣谢](#鸣谢)

### 上手指南

###### 开发前的配置要求

1. Qt 5.15.2及适用版本的Qt-Creator。
2. Qt 5.15.2 MinGW 32-bit的Kit包

###### **安装步骤**

1. 安装Qt 5.15.2及适用版本的Qt-Creator社区版。
2. Clone the repo
3. 用Qt-Creator打开项目，选择.pro文件
4. 配置构建编译器，选择Qt 5.15.2 MinGW 32-bit

```sh
git clone https://github.com/llyiyi/QT-algo-work.git
```

### 文件目录说明

```bash
QT-algo-work
├─data                    #存放排序数据示例
├─font                    #存放界面字体
├─img                     #存放演示图片
├─new_data                #存放大排序数据集
├─outputdata              #存放外部排序结果
└─rc                      #存放图标资源

```

### 部署

##### 直接使用（推荐）

下载最新的Releases版本，解压后点击exe运行即可。

##### 本地编译生成

利用Qt-Creator生成后运行。

### 版本控制

该项目使用 Git 进行版本管理。您可以在 repository 参看当前可用版本。

### 作者

原作者：bbdzs@https://github.com/bbdzs

此版本作者：
llyiyi@https://github.com/llyiyi
Xiong@https://github.com/xiongsp
harmor@https://github.com/harmor123

_您可以在贡献者名单中参看所有参与该项目的开发者。_

### 版权说明

该项目签署了 GNU GPLv3 授权许可，详情请参阅 [LICENSE](https://github.com/llyiyi/QT-algo-work/blob/main/LICENSE)

### 鸣谢

- [Sorting-Visualization-bbdzs](https://github.com/bbdzs/Sorting-Visualization-bbdzs)

<!-- links -->

[your-project-path]: llyiyi/QT-algo-work
[contributors-shield]: https://img.shields.io/github/contributors/llyiyi/QT-algo-work.svg?style=flat-square
[contributors-url]: https://github.com/llyiyi/QT-algo-work/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/llyiyi/QT-algo-work.svg?style=flat-square
[forks-url]: https://github.com/llyiyi/QT-algo-work/network/members
[stars-shield]: https://img.shields.io/github/stars/llyiyi/QT-algo-work.svg?style=flat-square
[stars-url]: https://github.com/llyiyi/QT-algo-work/stargazers
[issues-shield]: https://img.shields.io/github/issues/llyiyi/QT-algo-work.svg?style=flat-square
[issues-url]: https://img.shields.io/github/issues/llyiyi/QT-algo-work.svg
[license-shield]: https://img.shields.io/github/license/llyiyi/QT-algo-work.svg?style=flat-square
[license-url]: https://github.com/llyiyi/QT-algo-work/blob/main/LICENSE
