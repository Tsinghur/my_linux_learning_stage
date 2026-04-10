## Day40——Git

### Git常用命令

1. **git clone——下载仓库内容**

   ```c
   #下载远程仓库的内容，并且在本地创建一个和远程仓库名同名的文件夹，下载的时候, 路径在哪, 就直接下载到那个地方
   git clone https://gitee.com/boldness2012/xxxx.git
   
   #下载到指定的文件夹中(完全不建议)
   git clone https://gitee.com/boldness2012/xxxx.git  dirName
   ```

2. **git status——查看工作区和缓冲区和本地仓库中的变化**

3. **git add——把工作区中的变化提交到缓冲区**

   ```c
   #所有文件: 把所有文件的变化都从工作空间提交到缓冲区
   git add .
   
   #文件的名字:  把指定的文件名文件在工作空间发生的变化提交到缓冲区
   git add fileName
   
   #文件的类型: *通配符
   git add *.c  #把所有以.c结尾的文件的变化, 从工作空间提交到缓冲区
   ```

4. **git commit——把缓冲区变化内容提交到本地仓库**

   ```c
   #提交
   git commit -m "备注/注释"
   
   ## msg:msg信息一般要有统一的格式和意义(参照公司怎么写), 反正不建议乱写和简写(一定要注意) 
   ## 例如：Auther:ciggar Date:20220528 msg:xxx
   ```

5. **git log——查看仓库中的所有的版本信息**

   ```c
   #列出版本的详细信息
   git log
   
   #列出压缩格式的所有分支的提交树 (--oneline: 每个版本压缩成一行)(--all:所有分支)(--graph:以分支树的形式展示)
   git log --oneline --all --graph
   
   #如果出现历史记录过长: Q回车退出
   ```

6. 

### 笔记

1. git中Y型分支是什么？哪些情况下会有问题?如何解决Y型分支的冲突
2. 多人合作写项目的时候，如何防止冲突？如果发生冲突，如何解决