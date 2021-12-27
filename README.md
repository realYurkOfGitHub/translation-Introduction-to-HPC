# 高性能计算中文翻译计划 - Introduction to High Performance Scientific Computing (Victor Eijkhout)
为 Eijkhout 教授的Introduction to HPC教材提供中文翻译、 PPT和Lab。
> 本项目的原版教材地址可以在 [教程原文](https://tinyurl.com/vle394course)中下载，你还可以访问 Eijkhout 教授的[开源repository](https://bitbucket.org/VictorEijkhout/scientific-computing-public/src/master/)下载PPT、Lab源文件。

## 引言
科学计算是一门涉及多个领域，对问题过程进行综合建模，并使用计算机求得定量结果的交叉学科。它将某一门具体的学科转变为数值计算问题，并高效构建数学中的解决办法。这清楚地表明了科学计算涉及的三个科学分支：

- 应用数学：真实世界的数学建模。应用数学常常隐式地描述真实世界的运行规律，例如偏微分方程。为了取得实际的具体成果，我们需要采取一种建设性的办法。
- 数值分析提供了科学模型的算法思考。它提供了一个建设性的方法来解决隐式模型，并分析了成本和稳定性。
- 计算采用数值算法，并分析实际实现它们的效果，而非假设的计算引擎。

有人可能会说，当现实世界现象的数学被要求具有建设性时，“计算”本身就成为了一个领域。也就是说，从证明解的存在到实际求得解，算法本身成为一个研究对象，而不仅仅是一个研究工具。计算机发明后，对算法的研究变得尤为重要。人们用时间开销重新定义了数学运算，算法的复杂度进而成为了一个新的研究领域；计算不再以“实数”的形式进行，而是以有限位串的形式进行，因此我们还需要研究算法的准确性。事实上，早在机械计算器诞生之初，这种趋势就已经出现了萌芽。

科学计算重点关注的是效率。虽然部分科学家仅仅以；“找到一种解决办法”作为研究目的，但在计算中，决方案往往需要时效性。基于此，本书中我们将非常具体地讨论算法和硬件的效率——因为高性能计算不仅是硬件上的高效实用，更有对算法本身的研究。

本书涵盖了一名合格的计算科学家需要掌握的基础知识，可以作为研究生或高级本科生的教材；或者您也可以把它作为额外的参考，也可以当作阅读练习的材料。

Victor Eijkhout eijkhout@tacc.utexas.edu Research Scientist

Texas Advanced Computing Center

The University of Texas at Austin

## 课程目录：
### 第一部分：理论
1. [单处理器运算](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第一部分：理论)
2. [并行计算](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第一部分：理论)
3. [计算机运算](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第一部分：理论)
4. [微分方程的数值处理](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第一部分：理论)
5. [数值线性代数](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第一部分：理论)
6. [高性能线性代数](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第一部分：理论)


### 第二部分：应用
1. [分子动力学](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第二部分：应用)
2. [组合算法](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第二部分：应用)
3. [图论分析](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第二部分：应用)
4. [N体问题](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第二部分：应用)
5. [蒙特卡洛方法](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第二部分：应用)
6. [机器学习](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第二部分：应用)

### 第三部分：附录
1. [线性代数](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)
2. [复杂度](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)
3. [偏微分方程](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)
4. [泰勒级数](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)
5. [最小化](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)
6. [图论](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)
7. [自动机理论](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)
8. [并行前缀](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第三部分：附录)

### 第四部分：Carpentry
0. [第四部分序言：写在HPC前](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
1. [Unix简介](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
2. [编译器和库](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
3. [Make与CMake](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
4. [源码控制与Git](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
5. [科学数据存储](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
6. [GNUplot](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
7. [代码规范](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
8. [Debug](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
9. [C、Fortran交互](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)
10. [位操作]()
11. [用于科学计算的LaTeX](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/第四部分：CARPENTRY)

### PPT
1. [高性能运算](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/PPT)
2. [高性能图论](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/PPT)
3. [高性能迭代](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/PPT)
4. [高性能线性代数](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/PPT)
5. [高性能并行计算](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/PPT)
6. [高性能缩放](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/PPT)
7. [高性能Unix](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/PPT)

### HPC 笔记四卷（整理自网络，非原书）
1. [硬件架构与基准测试](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/Notes%20for%20HPC)
2. [集群功率与系统监控](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/Notes%20for%20HPC)
3. [网络系统与存储系统](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/Notes%20for%20HPC)
4. [C语言常见优化策略](https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/tree/main/Notes%20for%20HPC)

## Intro to HPC课程内容链接
- [教材原文](https://web.corral.tacc.utexas.edu/CompEdu/pdf/stc/EijkhoutIntroToHPC.pdf)
- [PPT链接](https://bitbucket.org/VictorEijkhout/scientific-computing-public/src/master/slides/)
- [代码链接](https://bitbucket.org/VictorEijkhout/scientific-computing-public/src/master/code/)
- [附带资料](https://bitbucket.org/VictorEijkhout/scientific-computing-public/src/master/booksources/)

## 知乎专栏
[专栏：高性能计算翻译计划](https://www.zhihu.com/column/c_1448674165109125120)

## 作者信息
[Victor Eijkhout](https://pages.tacc.utexas.edu/~eijkhout/istc/istc.html)，[utexas.edu](https://en.wikipedia.org/wiki/University_of_Texas_at_Austin)高性能计算中心研究科学家，《高性能计算导论》作者。

- Cray公司的Kudos：“这是一个不朽的成就”；
- 英特尔公司：“这是一本优秀的、精心制作的、高度可读的书，是我多年来看到的对HPC状态最好的快照。”；
- Simulation Consultant：“优秀平衡理论和应用的高性能计算的全面概述”；
- Tennessee大学：“太棒了，科学计算的圣经”

## 教材使用
本书最初是为犹他州的SSC 335/394“科学和技术计算入门”课程设计的。现在，它被下列机构列为教科书或推荐阅读的内容：
| 学校/机构                            | 课程名称                                        |
| ------------------------------------ | ----------------------------------------------- |
| 密歇根州立大学                       | CMSE 822:并行计算(初级课程材料)                 |
| 马里兰大学                           | AMSC/ cmmsc 662(官方教材)                       |
| 犹他大学                             | CS6230科学计算和数据科学的并行计算和HPC(教科书) |
| 智利天主教教皇大学                   | IMT2112(教科书)                                 |
| 列日大学                             | INFO 0939(官方教材)                             |
| 新墨西哥大学                         | 数学/cs471(教科书)                              |
| 圣地亚哥州立大学                     | COMP 696(初级教科书)                            |
| 加州大学博尔德分校                   | APPM 4720/5720(教科书)                          |
| 德克萨斯大学埃尔帕索分校             | CPS 5401(教科书)                                |
| 纽约大学医学院                       | HPC生物医学(必读)                               |
| 洛斯阿拉莫斯信息科学与技术研究所     | 推荐阅读                                        |
| 杨百翰大学                           | IT 515R科学计算                                 |
| 戈登学院                             | CPS371                                          |
| 巴斯大学                             | CM30225                                         |
| 伊利诺伊理工大学                     | CS595                                           |
| ETH/瑞士国家超级计算机中心           |                                                 |
| 埃森杜伊斯堡大学                     | HPC 2014/2015                                   |
| 圣母大学                             | ACMS60212/40212                                 |
| 慕尼黑工业大学                       | 信息学V                                         |
| 田纳西大学                           | CS 594-4, M578                                  |
| 芝加哥大学计算学院                   |                                                 |
| 普渡大学                             | cs50100                                         |
| 新南威尔士大学                       | Math3101/Math5305                               |
| 康涅狄格大学                         | 物理学2200                                      |
| 塔尔图大学                           | 科学计算mtat08.010                              |
| 柏林维尔斯特拉斯应用分析与随机研究所 | 科学计算课程                                    |
| 匹兹堡大学                           | 数学1070/1080                                   |
| 英属哥伦比亚大学                     | COSC 407                                        |
| 佐治亚理工学院                       | CSE 8803HNC                                     |
| 萨班奇大学                           | cs531                                           |
| 巴斯大学                             | CM30225并行计算                                 |

## 勘误与指正
由于译者水平有限，部分内容可能略有偏差。如果您对内容有哪些疑问或批评指正，欢迎联系：realyurk@gmail.com

## 版权相关
高性能计算教材采用Creative Commons Attribution 3.0 Unported (CC BY 3.0) license，相关内容已获得原作者授权。

<img src="https://github.com/realYurkOfGitHub/translation-Introduction-to-HPC/blob/main/第一部分：理论/graphics/ccby.jpg" width="200px">
其他部分整理自网络，如有版权要求请联系本人
