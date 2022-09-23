# Best-Fit
操作系统课设三|最佳适应算法
# 详细设计
## 题目要求
实现最佳适应算法，最佳适应算法总是把既能满足要求，又是最小的空闲分区分配给作业。为了加速查找，该算法要求将所有的空闲区按其大小排序后，以递增顺序形成一个空白链。这样每次找到的第一个满足要求的空闲区，必然是最优的。但事实上并不一定。因为每次分配后剩余的空间一定是最小的，在存储器中将留下许多难以利用的小空闲区。同时每次分配后必须重新排序，这也带来了一定的开销。
## 程序逻辑流程图
![image](https://user-images.githubusercontent.com/66285048/191946616-f2a2a435-62fd-45b1-b424-d17985dba0af.png)  
## 在Windows环境下于Visual Studio 2019运行本程序
![image](https://user-images.githubusercontent.com/66285048/191946695-0dda9258-ac0f-4a65-83fd-68c5fcc6624a.png)  
## 在Linux环境下于终端运行本程序
![image](https://user-images.githubusercontent.com/66285048/191946776-2b63dc4b-9a45-400e-b412-ce51ff79ca30.png)  



