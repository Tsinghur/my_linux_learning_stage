# Day45

|              |        二叉树        |       **网格图**       |
| :----------: | :------------------: | :--------------------: |
| **递归入口** |        根节点        | 岛屿第一行最左边的陆地 |
| **递归方向** |    左儿子和右儿子    |   左右上下的相邻陆地   |
| **递归边界** | 空节点（或者叶节点） |  出界、遇到水或者旗子  |

## 112.[[Medium]200. 岛屿数量](https://leetcode.cn/problems/number-of-islands/description/)——“DFS”&==“图论”==

```c
// DFS
void dfs(char** grid, int m, int n, int i, int j){
    // 优先判断出界，然后判断!='1'(即包含了='0'或='2')，若为真则终止向下递归
    if(i < 0 || i >= m || j < 0 || j >= n || grid[i][j] != '1')
        return;

    // 若没出界且当前='1'，则：
    grid[i][j] = '2'; // 插旗，避免来回横跳即无限递归
    dfs(grid, m, n, i, j - 1); // 往左走
    dfs(grid, m, n, i, j + 1); // 往右走
    dfs(grid, m, n, i + 1, j); // 往上走
    dfs(grid, m, n, i - 1, j); // 往下走
}

int numIslands(char** grid, int gridSize, int* gridColSize) {
    int m = gridSize, n = gridColSize[0]; // m为行数，n为列数
    int ans = 0;
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            if(grid[i][j] == '1'){
                dfs(grid, m, n, i, j); // 把这个岛上所有陆地插满棋子，这样的话后面遍历到的'1'一定是新的岛的陆地，然后循环，也即只有在遍历到新的岛的第一个陆地时才会让ans自增
                ans++;
            }

    return ans;
}
```

## 113.[[Medium]994. 腐烂的橘子](https://leetcode.cn/problems/rotting-oranges/description/)——==”BFS-多源-按个处理/按层处理“==

```c
// BFS-多源广度优先搜索

// 写法1：每次处理一个，将坐标与时间分别存在两个单独的数组里
/*
同时从所有初始腐烂橘子出发：
    假设这些腐烂橘子刚开始是新鲜的，而有一个腐烂橘子(我们令其为超级源点)会在下一秒把这些橘子都变腐烂，而这个腐烂橘子刚开始在的时间是 −1 ，那么按照广度优先搜索的算法，下一分钟也就是第 0 分钟的时候，这个腐烂橘子会把它们都变成腐烂橘子，然后继续向外拓展，所以其实这些腐烂橘子是同一层的节点。“那么在广度优先搜索的时候，我们将这些腐烂橘子都放进队列里进行广度优先搜索即可”
*/
int orangesRotting(int** grid, int gridSize, int* gridColSize) {
    const int dr[4] = {0, 1, 0, -1}; // 改变列索引
    const int dc[4] = {1, 0, -1, 0}; // 改变行索引

    int R = gridSize, C = gridColSize[0]; // R为行数，C为列数
    int queue[R * C]; // 辅助队列，矩阵的行优先数组存储
    int front = 0, rear = 0; // 队首指针指向首元素，队尾指针指向下一个入队元素
    int breadth[R * C]; // 广度，即新鲜橘子的腐烂时间，初始为0
    memset(breadth, 0, sizeof(breadth));

    for(int r = 0; r < R; r++)
        for(int c = 0; c < C; c++)
            if(grid[r][c] == 2){
                int idx = r * C + c; // （"二维索引" -> "一维索引"）根据当前矩阵元素行列索引转换成其在辅助列队数组中的索引（计算方式：把握偏移量，一维推广到二维）
                queue[rear++] = idx; // 存储初始时的所有腐烂橘子的"一维索引"
            }

    int ans = 0;
    while(front != rear){ // 腐烂橘子的辅助队列不为空
        int idx = queue[front++]; // 取队首元素
        int r = idx / C, c = idx % C; // 一维索引 解析为-> 二维索引
        for(int k = 0; k < 4; k++){ // 腐烂上下左右相邻的橘子
            int new_r = r + dr[k];
            int new_c = c + dc[k];
            if( 0 <= new_r && new_r < R && 0 <= new_c && new_c < C && grid[new_r][new_c] == 1){
                grid[new_r][new_c] = 2;
                int new_idx = new_r * C + new_c;
                queue[rear++] = new_idx;
                breadth[new_idx] = breadth[idx] + 1; // 即此轮循环中的源头腐烂橘子
                ans = breadth[new_idx]; // 迭代不是递归，不存在回溯，所以不需要比较，一定是单向增长的，所以直接赋值更新即可
            }
        }
    }
    
    for(int i = 0; i < R; i++)
        for(int j = 0; j < C; j++)
            if(grid[i][j] == 1)
                return -1;
    
    return ans;
}

// 写法2：每次处理一层，将层与时间绑定
int orangesRotting(int** grid, int gridSize, int* gridColSize) {
    const int dr[4] = {0, 1, 0, -1}; // 改变行索引
    const int dc[4] = {1, 0, -1, 0}; // 改变列索引

    int R = gridSize, C = gridColSize[0];
    int fresh = 0; // 新鲜橘子个数
    
    // 先统计新鲜橘子个数，同时记录初始腐烂橘子
    int queue[R * C]; // 存储腐烂橘子的二维索引（按行优先转为一维）
    int front = 0, rear = 0;
    
    for(int r = 0; r < R; r++) {
        for(int c = 0; c < C; c++) {
            if(grid[r][c] == 1) {
                fresh++; // 统计新鲜橘子个数
            } else if(grid[r][c] == 2) {
                int idx = r * C + c;
                queue[rear++] = idx; // 初始腐烂橘子入队
            }
        }
    }
    
    int ans = 0;
    // 当还有新鲜橘子且队列不为空时继续
    while(fresh && front != rear) {
        ans++; // 经过一分钟
        
        int level_size = rear - front; // 当前层的橘子数量
        for(int i = 0; i < level_size; i++) {
            int idx = queue[front++];
            int r = idx / C, c = idx % C;
            
            for(int k = 0; k < 4; k++) {
                int new_r = r + dr[k];
                int new_c = c + dc[k];
                if(0 <= new_r && new_r < R && 0 <= new_c && new_c < C && grid[new_r][new_c] == 1) {
                    fresh--; // 新鲜橘子减少
                    grid[new_r][new_c] = 2; // 变为腐烂
                    int new_idx = new_r * C + new_c;
                    queue[rear++] = new_idx;
                }
            }
        }
    }
    
    return fresh ? -1 : ans;
}
```

## 114.[[Hard]154. 寻找旋转排序数组中的最小值 II](https://leetcode.cn/problems/find-minimum-in-rotated-sorted-array-ii/description/)——”二分“-7.升级

```c
// 二分-开区间
/*
    为什么一直要与nums[right]作比较，因为right向左移动之后，right右边的元素就已经被排除了，可以看作包含最小值的原数组中没有这些元素，所以right对应的值就是原数组分为两段的分界线即右半段最大的元素的索引，所以应该一直与之做比较————最主要的原因还是：因为 154 有重复，最后一个元素不可靠！例如：[2,2,1,2,2]，最右边是 2，但左边也全是 2，根本切不开数组！所以必须每次只看当前区间的最右侧 nums [right]，并且不断缩小 right，把不可靠的重复点去掉！最右元素 = 会失效的标尺，遇到 == 就废了，所以必须动态缩小 right，用当前区间的右端点做标尺。
*/
int findMin(int* nums, int numsSize) { 
    int left = -1, right = numsSize - 1; // 开区间 (-1, n-1)
    while (left + 1 < right) { // 开区间不为空
        int mid = left + (right - left) / 2; // 不易越界
        if(nums[mid] == nums[right])
            right--;
        else if (nums[mid] < nums[right])
            right = mid;
        else  // nums[mid] > nums[right]
            left = mid;
    } // ret=right

    return nums[right];
}

// 二分-闭区间
// int findMin(int* nums, int numsSize){
//     int left = 0, right = numsSize - 2; // 闭区间[0, n-2]
//     while(left <= right){ // 区间里至少两个数
//         int mid = left + (right - left) / 2;
//         if(nums[mid] == nums[right + 1])
//             right--;
//         else if(nums[mid] < nums[right + 1])
//             right = mid - 1;
//         else
//             left = mid + 1;
//     }

//     return nums[left];
// }
```

## 115.[[Medium]207. 课程表](https://leetcode.cn/problems/course-schedule/description/)——==“BFS-kahn算法-拓扑排序”==&==“DFS-三色标记法-检测环”==

```c
// 法一：BFS-kahn算法-拓扑排序(暴力扫描边集)——遍历课程看有无不需先修课程，最终判断未学习课程数量是否为0
// bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {
//     int in_degree[numCourses] = {};

//     // 统计每门课的先修课程数（即入度）
//     for(int i = 0; i < prerequisitesSize; i++)
//         in_degree[prerequisites[i][0]]++;
    
//     int courses_count = numCourses; // 目前还有多少门课未学习
//     int count = numCourses;
//     int queue[numCourses] = {}; // BFS-辅助队列
//     int front = 0, rear = 0;
//     for(int i = 0; i < numCourses; i++)
//         if(in_degree[i] == 0)
//             queue[rear++] = i;
//     while(front != rear){ // 课程数
//         // for(int i = 0; i < numCourses; i++){
//         int idx = queue[front++];
//         // if(in_degree[i] == 0){
//             // in_degree[i] = -1; // -1代表已经学习
//             courses_count--;
//             for(int j = 0; j < prerequisitesSize; j++)
//                 // if(prerequisites[j][1] == i)
//                 if(prerequisites[j][1] == idx)
//                     if(--in_degree[prerequisites[j][0]] == 0)
//                         queue[rear++] = prerequisites[j][0]; // 入度减少
//             // }
//         // }
//     }

//     return courses_count == 0;
// }

// 法二：DFS-三色标记法-检测环
// DFS 检测环，返回 true 表示找到环
bool dfs(int x, int** graph, int numCourses, int* colors) { // ！！！指针可以传入到数组，但是数组不可传入到指针！！！因为数组传入函数会退化
    colors[x] = 1; // 正在访问中

    for (int y = 0; y < numCourses; y++) {
        if (graph[x][y] == 0) 
            continue; // 没有边，跳过

        if (colors[y] == 1)
            return true; // 找到环

        if (colors[y] == 0 && dfs(y, graph, numCourses, colors))
            return true; // 找到环
    } // 要所有邻边colors[y]都=2才代表从x出发找不到环，一个无法说明什么，所以在for循环中不用判断colors[y]=2，因为所有均不=2就会跳出for循环即colors[x]=2也即return false，所以只需在里面判断有环的情况才会提前返回true即有环

    colors[x] = 2; // 访问完成，无环
    return false;
}

bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {
    // 分配邻接矩阵并初始化为 0
    int** graph = (int**)malloc(numCourses * sizeof(int*));
    for (int i = 0; i < numCourses; i++)
        graph[i] = (int*)calloc(numCourses, sizeof(int)); // calloc 自动初始化为 0
    // int graph[numCourses][numCourses] = {};

     // 构建邻接矩阵
    for (int i = 0; i < prerequisitesSize; i++) {
        int a = prerequisites[i][0]; // 课程 a
        int b = prerequisites[i][1]; // 依赖课程 b
        graph[b][a] = 1; // b -> a，表示要先学 b 才能学 a
    }

     // 颜色数组：0=未访问，1=访问中，2=已完成
    int* colors = (int*)calloc(numCourses, sizeof(int));
    // int colors[numCourses] = {};

    // 遍历所有节点
    for (int i = 0; i < numCourses; i++)
        if (colors[i] == 0 && dfs(i, graph, numCourses, colors)) {
            // 找到环，释放内存
            for (int j = 0; j < numCourses; j++) 
                free(graph[j]);
            free(graph);
            free(colors);
            return false; // 有环，无法完成
        }

    // 释放内存
    for (int i = 0; i < numCourses; i++) 
        free(graph[i]);
    free(graph);
    free(colors);
    return true; // 无环，可以完成
}

// 法三：BFS-kahn算法-拓扑排序（邻接表 >> 邻接矩阵 > 暴力扫描边集）
bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {
    //1.创建邻接表
    int** graph = (int**)malloc(sizeof(int*)*numCourses);       //二维指针，用来存放每个课程学完后可以继续学习的课程之间的关系
    int* graphColSize = (int*)calloc(numCourses,sizeof(int));      //该课程学完后可以继续学习的课程个数
    int* indegree = (int*)calloc(numCourses,sizeof(int));       //课程的入度

    //先统计每个节点有多少边
    for(int i = 0; i<prerequisitesSize; i++){
        int a = prerequisites[i][0];    //把课程关系拆开
        int b = prerequisites[i][1];
        graphColSize[b]++;      //b课程学完后后续可以学习的课程个数加一，也就是graph每行的列数也即邻接链表的结点数
        indegree[a]++;          //a课程的入度加一
    }

    //给每个节点分配邻接表空间
    for(int i = 0; i<numCourses; i++){
        graph[i] = (int*)malloc(sizeof(int)*graphColSize[i]);
        graphColSize[i] = 0; // 置零是为了下面建图的邻接表时作为索引重新统计一次，最终值与置零前相同
    }
    //建图的邻接表：b->a
    for(int i = 0; i<prerequisitesSize; i++){
        int a = prerequisites[i][0];
        int b = prerequisites[i][1];
        graph[b][graphColSize[b]++] = a;    //b的graph图中，第1个元素即为a
    }
    
    //队列：存入入度为0的课程
    int* queue = (int*)malloc(sizeof(int)*numCourses);
    int front = 0, rear = 0;
    for(int i = 0; i<numCourses; i++){
        if(indegree[i] == 0){
            queue[rear++] = i;
        }
    }

    //3.拓扑结构
    int count = 0;
    while(front<rear){
        int cur = queue[front++];       //取队列中头元素的课程进行学习
        count++;
        for(int i = 0; i<graphColSize[cur]; i++){
            int next = graph[cur][i];
            indegree[next]--;       //把需要先学习i的课程的入度减一

            if(indegree[next] == 0){        //如果当前课程的入度为0，那么就加入队列
                queue[rear++] = next;
            }
        }
    }

    //4.释放内存
    for(int i = 0; i<numCourses; i++){
        free(graph[i]);
    }
    free(graph);
    free(graphColSize);
    free(indegree);
    free(queue);

    return count == numCourses;
}
```

## 116.[[⭐Medium+]208. 实现 Trie (前缀树)](https://leetcode.cn/problems/implement-trie-prefix-tree/description/)——“设计”&==“字典树(前缀树)”==&“二十六叉树”

```c
// 字典树(前缀树)-二十六叉树:每个节点最多有 26 个子节点的树状数据结构
// Trie是整个树，不应该是其中的某个节点，应该Trie类中有头节点属性，并定义节点的结构，通过节点迭代进行前缀树遍历才是符合逻辑的
#define ALPHABET_SIZE 26 // 字母表尺寸

typedef struct node{
    struct node* son[ALPHABET_SIZE];
    bool end; // 到这里是否已经结束即是否是叶结点
} Node; // 字典树结点结构体

typedef struct {
    Node* root; // ！！！字典树根结点不对应字母，从它的第一代子结点开始对应单词的首字母
} Trie; // 字典树本体(即只包含一个根结点)

Trie* trieCreate() { // 创建字典树
    Trie* obj = malloc(sizeof(Trie));
    obj->root = calloc(1, sizeof(Node));
    return obj;
}

void trieInsert(Trie* obj, char* word) { // 向字典树中插入一个单词字符串
    Node* cur = obj->root;
    for(int i = 0; word[i]; i++){
        int c = word[i] -'a'; // 在字典树结点中的结构体指针数组中的索引，即ASCII字符 -> 索引
        if(!cur->son[c]) // 字典树中不存在这个单词的当前及之后的部分
            cur->son[c] = calloc(1, sizeof(Node)); // 无路可走那就造路
        cur = cur->son[c];
    }
    cur->end = true; // 单词的末尾字母结点end标志位置为true
}

// trieSearch与trieStartsWith函数的辅助函数
int trieFind(Trie* obj, char* word){
    Node* cur = obj->root;
    for(int i = 0; word[i]; i++){
        int c = word[i] - 'a';
        if(!cur->son[c]) // 字典树之后按照顺序没有这个单词的的所有字母即完整部分，道不同，不相为谋
            return 0;
        cur = cur->son[c];
    }

    return cur->end ? 1 : 2; // 走过同样的路（1=完全匹配，2=前缀匹配）
}

bool trieSearch(Trie* obj, char* word) { // 在字典树中搜索单词字符串
    return trieFind(obj, word) == 1;
}

bool trieStartsWith(Trie* obj, char* prefix) { // 在字典树中搜索此字符串作为前缀或单词时是否存在
    return trieFind(obj, prefix) != 0; // 注意：这里规定单词本身也是自己的前缀
}

// trieFree函数的辅助函数
void trieFreeNode(Node* node){
    if(!node)
        return;
    for(int i = 0; i < ALPHABET_SIZE; i++)
        trieFreeNode(node->son[i]); // 一直会先沿a->a->a->a递归下去，即能完全删除26叉树中除了根结点之后的所有可能存在的结点
    free(node);
}
void trieFree(Trie* obj) {
    trieFreeNode(obj->root);
    free(obj);
}

/*
 * Your Trie struct will be instantiated and called as such:
 * Trie* obj = trieCreate();
 * trieInsert(obj, word);
 
 * bool param_2 = trieSearch(obj, word);
 
 * bool param_3 = trieStartsWith(obj, prefix);
 
 * trieFree(obj);
*/
```

