# Day46

## 117.[[Medium]74. 搜索二维矩阵](https://leetcode.cn/problems/search-a-2d-matrix/description/)——“二分-一次/两次”&==“二维数组与一维数组转换”==&“排除法”

```c
// 法一：两次二分——把每行的最后一个数当做分块查找中的块索引，先查找块即行(若查不到则提前结束)，再去行中搜索究竟是否存在
// O(logm+logn)即O(logmn)
// bool searchMatrix(int** matrix, int matrixSize, int* matrixColSize, int target) {
//     int m = matrixSize, n = matrixColSize[0];
    
//     int left = -1, right = m;
//     while(left + 1 < right){ // 开区间(-1,m)
//         int mid = left + (right - left) / 2;
//         if(matrix[mid][n-1] < target)
//             left = mid;
//         else
//             right = mid;
//     } // ret = right
//     if(right == m) // 没有能容下target的块(行)即target大于矩阵中所有数
//         return false;

//     int row = right;
//     if(row < 0 || row >= m)
//         return false;
//     left = -1, right = n;
//     while(left + 1 < right){ // (-1,n)
//         int mid = left + (right - left) / 2;
//         // <1>提前返回
//         // if(matrix[row][mid] == target) // 在重复元素中寻找元素可直接判断然后提前返回，若是在重复元素中找最终插入位置则还需走完全程
//         //     return true;
//         // else if
//         if(matrix[row][mid] < target)
//             left = mid;
//         else
//             right = mid;
//     } // ret = right
//     // return false; // <1>提前返回

//     // if(right == n) // 即target在可能存在的行中不存在
//     //     return false;
//     // 将上面判断与返回判断归并一起
//     return right != n && matrix[row][right] == target;
// }

// 法二：排除法
// O(m+n)
// 观察矩阵右上角元素可得出一个规律：左边的所有元素小于等于它，下边的所有元素大于等于它，即其所处非严格递增行与严格递增列；所以将当前剩余矩阵的右上角元素作为基准，若是target大于它则删去此行，若是target小于它则删去此列
// bool searchMatrix(int** matrix, int matrixSize, int* matrixColSize, int target) {
//     int m = matrixSize, n = matrixColSize[0];
//     // 注意是右上角，所以初始时行数应是最小，列数应是最大
//     int i = 0, j = n - 1;
//     while(i < m && j >= 0){ // 行索引合法范围[0,m-1]，列索引[0,n-1]
//         if(matrix[i][j] == target)
//             return true;
//         else if(matrix[i][j] < target)
//             i++;
//         else
//             j--;
//     }
//     return false;
// }

// 法三：一次二分——将矩阵展开成数组，然后直接二分
// O(logmn)
// 一维索引 i=r*n+c <<<——————>>> r=i/n,c=i%n 二维索引
bool searchMatrix(int** matrix, int matrixSize, int* matrixColSize, int target) {
    int m = matrixSize, n = matrixColSize[0];
    int left = -1, right = m * n;  // 开区间(-1,m*n)
    while(left + 1 < right){
        int mid = left + (right - left) / 2;
        int x = matrix[mid / n][mid % n];
        // if(x == target)
        //     return true;
        // else if
        if(x < target)
            left = mid;
        else
            right = mid;
    }
    // return false;
    return right != (m*n) && matrix[right / n][right % n] == target;
}
```

## 118.[[Hard+]4. 寻找两个正序数组的中位数](https://leetcode.cn/problems/median-of-two-sorted-arrays/description/)——==“二分-究极”==&“双指针”

**review**

> 二分类型的变种题目都有一个核心：那就是排除法，根据循环不变量来进行对错误数据的一步步排除，平均每次排除当前可能正确的元素集合的一半，这就是二分的本质（这只是二分过程的本质）
>
> ！！！二分算法的本质就是为了找到最后一次满足循环不变量的状态即边界（这也是二分的目的），具体体现见118.法三

![image-20260409205901278](D:\Typora Picture\image-20260409205901278.png)

![image-20260409205926649](D:\Typora Picture\image-20260409205926649.png)

![image-20260409205933343](D:\Typora Picture\image-20260409205933343.png)

![image-20260409220619653](D:\Typora Picture\image-20260409220619653.png)

```c
// 法一：暴力解法-合并+排序——O((m+n)log(m+n))，不满足题目规定：O(log(m+n))
// int cmp(const void* a, const void* b){
//     int num1 = *(int*)a;
//     int num2 = *(int*)b;
//     return num1 - num2;
// }

// double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
//     int m = nums1Size, n = nums2Size;
//     int* tmp = calloc(m + n, sizeof(int));
//     memcpy(tmp, nums1, m * sizeof(int));
//     memcpy(tmp + m, nums2, n * sizeof(int));

//     qsort(tmp, m + n, sizeof(int), cmp);

//     double ans;
//     int mid = (m + n - 1) / 2;
//     if(((m + n) & 1) == 1)
//         ans = tmp[mid];
//     else
//         ans = (tmp[mid] + tmp[mid + 1]) / 2.0;

//     return ans;
// }

// 法一的优化：边合并边排序-归并：辅助数组/双指针计数法——T=O(m+n),S=O(m+n)/O(1)
// 待实现

// 法二：双指针
// 就是在a中移动i，b中移动j，写法一种是在循环中用if判断，判断成功则直接返回答案，写法二则是设置边界条件，若是一直不满足“条件”就在while中循环调整，然后在跳出while时也就是刚好满足“条件”的时候。（总结：写法一：主动检查两个条件，命中即停。写法二：只盯一个条件，把它“逼”到不成立为止，利用单调性让另一个条件自动成立。）
// #define MY_MAX(a, b) ((a) > (b) ? (a) : (b))
// #define MY_MIN(a, b) ((a) < (b) ? (a) : (b))
// double findMedianSortedArrays(int* nums1, int m, int* nums2, int n) {
//     if (m > n) {
//         // 利用递归交换 nums1 和 nums2
//         return findMedianSortedArrays(nums2, n, nums1, m);
//     }

//     int* a = malloc((m + 2) * sizeof(int));
//     int* b = malloc((n + 2) * sizeof(int));
//     a[0] = b[0] = INT_MIN; // 最左边插入 -∞
//     a[m + 1] = b[n + 1] = INT_MAX; // 最右边插入 ∞
//     memcpy(a + 1, nums1, m * sizeof(int)); // 数组没法直接插入，只能 copy
//     memcpy(b + 1, nums2, n * sizeof(int));

//     // 枚举 nums1 有 i 个数在第一组
//     // 那么 nums2 有 j = (m + n + 1) / 2 - i 个数在第一组
//     int i = 0, j = (m + n + 1) / 2;
//     while (true) {
//         if (a[i] <= b[j + 1] && a[i + 1] > b[j]) { // 写 >= 也可以
//             int max1 = MY_MAX(a[i], b[j]); // 第一组的最大值
//             int min2 = MY_MIN(a[i + 1], b[j + 1]); // 第二组的最小值
//             free(a);
//             free(b);
//             return (m + n) % 2 ? max1 : (max1 + min2) / 2.0;
//         }
//         i++; // 继续枚举
//         j--;
//     }
// }

// 法三：二分——O(log min(m,n))
#define MY_MAX(a, b) ((a) > (b) ? (a) : (b))
#define MY_MIN(a, b) ((a) < (b) ? (a) : (b))
double findMedianSortedArrays(int* a, int m, int* b, int n) {
    // 利用递归交换nums1和nums2，从而将nums1变为较短的数组
    if(m > n)
        return findMedianSortedArrays(b, n, a, m);

    // 循环不变量:
    // nums1[left] <= b[j+1]
    // a[right] > b[j+1]
    int left = -1, right = m;
    while(left + 1 < right){ // 开区间(left,right)不为空
        int i = left + (right - left) / 2;
        int j = (m + n + 1) / 2 - i - 2; // 即-(i + 1) - 1 也即完整左半段总数 - a中左半段数量 - 1（注意i与j此时是下标，+1才代表数量）
        if(a[i] <= b[j + 1])
            left = i; // 缩小二分区间为 (i, right)
        else
            right = i; // 缩小二分区间为 (left, i)
    } // ！本质上就是找最后一个满足条件("第一组的最大值不超过第二组的最小值")的位置与第一个不满足条件的位置即left与right，这里ret = right就是二分里第一个大于target的位置，而target就是b[j+1]也就是说最终的right也就是i+1就是第一个a中大于此时b[j+1]的位置（此时是刚好跳出循环也就是说刚好最后一次满足循环不变量的状态就是说如果再变化i也就是此时的left<为什么是left，因为二分取中值一定是偏左的且此时left+1=right所以mid即i就是left> 如果再继续向右更新那么就刚好开始不满足条件了，所以说我们这里真正需要的是left而不是经典的二分ret=right<这里的right更像是一个辅助>）（补充：为什么结束时刚好是最后一次满足，且为什么再更新一次就刚好不满足，因为我们在二分时，主动把 right 定义在了“不满足条件”的区域。这不是推导出来的，而是人为设定的规则。所以结束时a[right] > b[j + 1]即下一次的a[i=left+1=right] > b[j+1]>=b[(j-1)+1]，也就是a[new_i] > b[new_j + 1]即不满足题设条件也就是说这时的划分是错误的，左半段的所有值不是全小于等于右半段所有值的）
    // 二分算法的本质(目的)：找到最后一次满足循环不变量的状态！！！
//-----------------------------------------------------------------
    int i = left;
    int j = (m + n + 1) / 2 - i - 2;

    int ai = i >= 0 ? a[i] : INT_MIN; // 无穷值的设置是代表这个值是废值，无法与另一个比较值相提并论，即一定会将三目运算符的返回值位置礼让给对方
    int bj = j >= 0 ? b[j] : INT_MIN;
    int ai1 = i + 1 < m ? a[i + 1] : INT_MAX; // ai1就是a[i + 1]，变量命名规则如此只能这样表示
    int bj1 = j + 1 < n ? b[j + 1] : INT_MAX;

    int max_ai_bj = MY_MAX(ai, bj);
    int min_ai1_bj1 = MY_MIN(ai1, bj1); 
//-----------------------------------------------------------------
    return (m + n) % 2 ? max_ai_bj : (max_ai_bj + min_ai1_bj1) / 2.0;
}
/*
两种边界情况，方便更好理解二分逻辑
    首先有一点需要明确，i+1与j+1代表第一组在a与b中的个数，且j是由i算出来的，i才是二分的结果，所以不必担心j的正确性，接下来由具体情况来验证
    1.初始时a中所有元素都小于b中所有元素
        假设：
            a = [1, 2] (长度 m=2)
            b = [3, 4, 5, 6] (长度 n=4)
        二分初始状态：
            left, right = -1, 2 # 开区间 (-1, 2)
        过程：
            left 会一路向右移动，从 -1 走到 0，再走到 1
            right 始终保持不动（始终是初始值 2），因为条件从未失败过
    2.初始时a中所有元素都大于b中所有元素
        假设：
            a = [5, 6] (长度 m=2)
            b = [1, 2, 3, 4] (长度 n=4)
        二分初始状态：
            left, right = -1, 2 # 开区间 (-1, 2)
        过程：
            无法进入循环，最终left = -1，即代表a中有i=left + 1 = 0个第一组元素，第一组完全由 b 的前面元素构成，j由公式计算出
*/
```

