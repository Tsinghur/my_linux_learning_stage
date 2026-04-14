## Day43_SQL3_多表与C-API与事务

### 多表问题

#### 1.多表设计(多表理论)

1. **一对一**

   指两个表（或多个表之间）的数据存在一一对应的关系

   > 用户和用户详情
   > 商品和商品详情

2. **一对多**

   指两个表（或多个表之间）的数据，存在表A中的一条数据对应表B中的多条数据，表B中的一条数据对应表A中的一条数

   > 用户和订单
   > 班级和学生

3. **多对多**

   存在两个表表A和表B，存在表A中的一条数据对应表B中的多条数据，表B中的一条数据对应表A中的多条数据。

   > 订单和商品
   > 一个商品可能存在于多个订单,  一个订单中可能买了多个商品 
   >
   > 剧本和演员
   > 一个演员可能出演了多个剧本,  一个剧本中可能包含多个演员

#### 2.数据库设计范式

和数据完整性不同，数据库的设计范式更偏向于表设计的维度来看待数据的存储. 其存在的目的也是为了, 在维护或者操作数据库中数据:

<span style=color:red;font-size:20px>  **1、希望在数据库中数据存储更规整 2、希望操作数据的时候效率更高**</span>

==**三大范式的核心目标**==

1. **消除重复数据——原子性**
2. **消除部分依赖——唯一性**
3. **消除传递依赖——不冗余**

- ==**第一范式：原子性**==

  - 每一列**不能再拆**（原子性）
  - 每一行**能唯一区分开**（不能有完全一样的两行）

  在设计表的时候, 应该每列保持原子性。 如果数据库中的所有字段都是<span style=color:red;background:yellow;font-size:20px>**不可分割的原子值**</span>，则说明该数据库满足第一范式，比如：地址。

  第一范式：我们在设计表的时候，应该考虑之后业务的变化，来尽量让每一列的数据保持原子性。

  **反例：**

  ```sql
  id   name    hobby
  1    张三    篮球,足球,游戏
  2    李四    看书
  -- -------------------------------
  # 满足第一范式后
  id   name    hobby
  1    张三    篮球
  1    张三    足球
  1    张三    游戏
  2    李四    看书
  # 每一行都是独立、唯一、不可再分
  ```

- ==**第二范式：唯一性**==

  - 必须先满足**第一范式**
  - **所有非主键字段，必须依赖【完整主键】，不能只依赖主键的一部分**
  - 避免：**部分依赖 → 造成数据冗余、重复、无法唯一确定**

  数据的唯一性。 要求表中数据有<span style=color:red;background:yellow;font-size:20px>**唯一标识**</span>，不存在部分依赖。非主键字段必须完全依赖整个主键，而不是部分主键，通过主键来唯一标识一个用户(满足唯一性)

  注意: 通过name+nickname+province+city+county组合标识一个用户(不满足唯一性)

  **反例：**

  ```sql
  学号  课程号  姓名   课程名
  001  101    张三   数学
  001  102    张三   语文
  主键是：(学号，课程号)
  但姓名只依赖学号，不依赖课程号 → 部分依赖,导致：张三重复出现，数据冗余，无法唯一确定一个人。
  -- ---------------------------------------------------------------
  若是要满足第二范式，则拆表：
  # 学生表：
  学号  姓名
  001  张三
  # 课程表：
  课程号  课程名
  101    数学
  102    语文
  # 成绩表：
  学号  课程号
  001  101
  001  102
  # 每一行都唯一、不重复、不冗余
  ```

  

- ==**第三范式：不冗余**==

  - 先满足前两个范式即先满足**第二范式**
  - 不能有：A → B → C 这种传递依赖
  - 所有非主键必须**直接依赖主键**，而不是依赖其他非主键
  - 保证：**通过主键能唯一确定所有字段**

  <span style=color:red;background:yellow;font-size:20px>**字段不要冗余**</span>。(消除表中非主键字段间的依赖: 即:要求每个非主键字段只依赖于主键，而不依赖于其他非主键字段)

  **反例：**

  ```sql
  学号  姓名  学院  学院电话
  001  张三  计算机  123456
  # 学院电话 依赖学院，不直接依赖学号
  # → 传递依赖：学号 → 学院 → 学院电话
  # 导致：学院电话重复存储，无法唯一确定学院信息
  -- --------------------------------------
  若要满足第三范式，则需拆表
  # 学生表
  学号  姓名  学院编号
  001  张三   01
  # 学院表
  学院编号  学院名   电话
  01      计算机  123456
  # 所有信息都只依赖主键，没有传递依赖，唯一性彻底保证
  ```

#### 3.多表查询(连表查询)

##### 1.交叉连接

交叉连接其实就是求多个表的<span style=color:red;font-size:20px>**笛卡尔积**</span>

```sql
SELECT <字段名> FROM <表1> CROSS JOIN <表2> [WHERE子句]

select * from student cross  join  equip;
select * from student cross  join  equip  where student.id = equip.student_id;
# 两个表的笛卡尔积，返回结果数量就是两个表的数据行相乘。
# 如果每个表有1000行，那么返回结果的数量就有1000×1000=1000000行。
```

仅交叉连接的结果没有太多实际的使用意义

> 注：
>
> 交叉连接（笛卡尔积）既不是排列，也不是组合，它就是「两个集合的有序配对」，数学上叫笛卡尔积。
>
> **笛卡尔积（交叉连接）**：
>
> 左表一条 + 右表一条，**固定左在前、右在后**，不会反过来，也不会内部乱排。

##### 2.自然连接

没什么用

```sql
# 自然连接是基于两个表之间的共同列来自动匹配并组合数据。
# 自然连接将结果集限制为只包括两个表中`具有相同值`的列(并且在结果集中把重复的列去掉)。在使用自然连接时，不需要指定连接条件，而是根据两个表中具有相同名称和数据类型的列进行匹配。 (注意: 有些数据库不支持自然连接， 比如SQLServer )

select * from student natural join class;
```

##### 3.内连接——本质：交叉连接+过滤

​		INNER JOIN 就是**只保留两边都能匹配上的数据**，不匹配的，直接丢掉。
​		**两个表行数不一样，才是常态**，内连接本来就是为这种情况设计的。INNER JOIN 不关心两个表原来有多少行，它只关心：有多少行能匹配上。最终结果行数 = **能匹配上的行数**，可能比两个表都少，也可能在中间，**不可能比两个表都多**（不考虑一对多重复的情况）

==在 SQL 里，`JOIN` 就等于 `INNER JOIN`，`INNER` 是可以省略不写的==

```sql
SELECT <字段名> FROM <表1> INNER JOIN <表2> [ON子句]

#显示内连接
select * from  student inner join equip on student.id = equip.student_id;

# 隐式内连接: 不建议这样写(这是早期的sql语法中内连接的一种写法)
select * from student , equip  where  student.id = equip.student_id;
```

##### 4.左/右外连接

**不加where时**，主表的行一条都不会少，一定会全部显示

显示数量：**最少 = 主表行数**，可能更多，但绝对不会更少

主副表的问题

> 假设A和B表进行连接，AB两张表一个表示主表，另一个是副表; 查询数据的时候,   以主表中的数据为基准，匹配副表对应的数据;   当副表中的数据没有能和主表对应数据相互匹配的数据，副表匹配位置自动填充null。

```sql
SELECT <字段名> FROM <表1> LEFT OUTER JOIN <表2> <ON子句>
SELECT <字段名> FROM <表1> RIGHT OUTER JOIN <表2> <ON子句>


select * from student left outer join equip on student.id = equip.student_id;
select * from student right outer join equip on student.id = equip.student_id;
select * from equip right outer join student on student.id = equip.student_id;
select * from equip left outer join student on student.id = equip.student_id;
-- outer可省略: 工作中多数人省略了outer
select * from student left join equip on student.id = equip.student_id;
select * from student right join equip on student.id = equip.student_id;
```

##### 5.自连接

```sql
#自连接是指在同一个表中，使用不同的别名将它们连接到一起。

#查询数学成绩低于林冲的数学成绩的人的信息
select t1.* from score t1,score t2 where  t2.id = 1 and  t1.chinese < t2.chinese
```

##### 6.子查询(嵌套查询)

子查询也叫**嵌套查询**( 在某个操作中(删除/修改/查找), 用到了另外一个查询的结果) 

是指在WHERE子句或FROM子句中又嵌入SELECT查询语句

```sql
SELECT <字段名> FROM <表|子查询> WHERE <IN| NOT IN | EXISTS | NOT EXISTS > <子查询>

select * from  student where  id in (select student_id from equip);
select * from  student where  id  not in (select student_id from equip where student_id !=  "");

select * from  student where exists (select * from equip where student_id = 11);
select * from  student where not exists (select * from equip where student_id = 11);
select * from  student where exists (select * from equip where student_id = 11)  and id = 5;
# 在MySQL每次查询数据的结果集都是一个新的临时表。
```

##### 7.联合查询

联合查询**合并**两条查询语句的查询结果.

联合查询**去掉**两条查询语句中的**重复数据行**，然后返回合并后没有重复数据行的查询结果。

```sql
SELECT <字段名> FROM <表> UNION  SELECT <字段名> FROM <表>

select * from  score  where chinese >= 90 union select * from  score  where math >= 90;
```

### C的API

#### 相关函数

1. **mysql_init——初始化连接**

   ```c
   MYSQL *mysql_init(
       MYSQL *mysql // 一个指向MYSQL结构的指针,通常传入NULL，函数会为你分配一个新的MYSQL结构。
   			    // 如果你已经有一个MYSQL结构的空间，也可以传入该结构的指针，函数将在该结构上进行初始化。
   );
   // 返回值: 成功时，返回一个指向MYSQL结构的指针。如果内存分配失败，返回NULL。
   ```

   > mysql_init函数线程不安全: 所以当在一个进程中的多个线程中准备同时创建多个数据库连接的时候, 建议先加锁(connect之后再解锁)

2. **mysql_real_connect——建立到Mysql服务器的连接**

   ```c
   MYSQL *mysql_real_connect(
       MYSQL *mysql,        // 指向MYSQL结构的指针
       const char *host,    // 数据库服务器的主机名或IP地址(传递NULL或"localhost"将尝试连接到本地机器上的服务器。)
       const char *user,    // 用于登录数据库的用户名
       const char *passwd,  // 与用户名相对应的密码 
       const char *db,      // 要连接的数据库名
       unsigned int port,   // 数据库服务器的端口号。如果指定为0，将使用默认端口（通常是3306）
       const char *unix_socket, // 连接本地MySQL服务器时Unix套接字文件路径。可以填NULL
       unsigned long client_flag // 用于控制连接行为的标志。填0即可。
   );
   // 成功时，返回MYSQL结构的指针。失败时，返回NULL。
   ```

3. **mysql_query——执行增删改查的SQL语句**

   ```c
   int mysql_query(
      MYSQL *mysql, // 建立连接的MYSQL结构的指针
      const char *query // 要执行的SQL语句的字符串(这个SQL字符串不应以分号结尾，并且必须是以空字符结束)
   );
   // 返回值:成功返回0。失败返回非0值
   ```

4. **处理结果**

   1. **mysql_store_result——获取返回结果**

      ```c
      // 获取由mysql_query()函数返回的结果
      MYSQL_RES *mysql_store_result(
      	MYSQL *mysql // 建立连接的MYSQL结构的指针
      );
      // 返回值:
      // 成功：返回一个指向MYSQL_RES结构的指针，该结构代表了从服务器返回的结果集。通过这个结构，可以进一步获取查询结果
      // 失败：如果SQL没有返回数据(eg:INSERT/UPDATE/DELETE操作,或者SELECT没有找到结果,或者其它错误)，则返回NULL
      ```

   2. **mysql_fetch_row——从结果中取出一行**

      ```c
      // 从结果当中取出一行
      MYSQL_ROW mysql_fetch_row(
          MYSQL_RES *result // 指向MYSQL_RES结构的指针
      );
      // 返回值: 返回MYSQL_ROW类型(一个字符串数组，每个元素对应结果集中一行的一个字段的值)。
      // 如果字段值为NULL，相应的数组元素也将是NULL。
      // 如果没有更多行了,或结果集为空，函数返回NULL。
      ```

   3. **mysql_num_rows——获取结果行数**

      ```c
      // SQL语句结果的行数
      my_ulonglong mysql_num_rows(
          MYSQL_RES *result // 指向MYSQL_RES结构的指针
      );
      // 返回值:结果集的行数(my_ulonglong: 是一个无符号的64位整数); 如果result是NULL函数返回0。
      ```

   4. **mysql_num_fields——获取结果列数**

      ```c
      // SQL语句结果的列数
      unsigned int mysql_num_fields(
          MYSQL_RES *result // 指向MYSQL_RES结构的指针
      );
      // 返回值:返回结果集中的列数; 如果result是NULL函数返回0
      ```

   5. **mysql_free_result——释放数据结构MYSQL_RES占据的内存空间**

      ```c
      // 释放数据结构MYSQL_RES占据的内存空间
      void mysql_free_result(
          MYSQL_RES *result // 指向MYSQL_RES结构的指针
      );
      ```

5. **mysql_close——关闭与MySQL的连接**

   ```c
   void mysql_close(
       MYSQL *mysql // 建立连接的MYSQL结构的指针
   );
   ```

6. **mysql_error——检测函数出错，诊断了解为什么一个特定的MySQL操作（如连接、查询等）失败**

   ```c
   const char *mysql_error(
       MYSQL *mysql // MYSQL结构的指针
   );
   // 返回值: 描述最近一次API调用失败原因
   ```

#### 代码示例

1. **select查询**

   ```c
   
   ```

2. **update修改**

   ```c
   
   ```

### 事务

<span style=color:red;background:yellow;font-size:20px>**事务是组成逻辑的一组SQL操作，这个操作的各个单元或者语句，要么都成功执行，要么都不成功执行**</span>

<span style=color:red;font-size:20px>**构成事务的三个操作：开启事务、回滚(非必要)、提交**</span>

```sql
# 开启事务
# start transaction   #也可以使用begin开启事务
begin;    # 开启事务

#执行sql操作
update account set money = money+200 where name like '%黄四郎%';

update account set money = money+200 where name like '%家族2%';
update account set money = money+200 where name like '%家族3%';
update account set money = money+200 where name like '%家族4%';

#事务回滚
rollback; -- 回滚事务之后，之前(到begin)所有的操作都会失效。

update account set money = money-800 where name like '张麻子';

#提交事务
commit; # 提交事务之后，从开启事务到提交事务之间所有的操作才会生效。
```

<span style=color:red;font-size:20px>**注意: 开启事务之后, 要注意提交(要么rollback, 要么commit，即rollback和commit是完全对立的但但核心功能相同-即结束事务)**</span>

#### 事务特性ACID

==**做了一堆SQL操作：这些sql操作我们希望是不可拆分的**==

<span style=color:red;background:yellow;font-size:20px>**(1)原子性atomicity**</span>

一个事务, **这个事务整体的所有sql操作都要必须要被视为一个不可分割的最小单元**, **整个事务中的操作要么全部提交成功, 要么全部失败回滚**, 对于一个事务来说, 不可能只执行其中的一部分操作, 这就是事务的原子性.(这是从操作角度来谈失误: 操作要么都成功, 要么都失败, 不可切割)

<span style=color:red;background:yellow;font-size:20px>**(2)一致性consistency**</span>

**数据库总是从一个一致性的状态转换到另一个一致性的状态**. 如前面的例子, 所谓一致性, 就是要么扣款和增加都成功, 要么未成功发生扣款的同时也未发生金额增加, 数据(金钱的总量是要前后一致的)是一致性变化的. (从事务的结果, 数据角度来谈:  **数据是从一个状态, 通过事务, 要么不变, 要么一致性的演变到另外一种状态**)

<span style=color:red;background:yellow;font-size:20px>**(3)隔离性isolation**</span>

就是**事务和事务之间的关联性**。注意事务之间的关联, 有可能强, 也有可能关联性弱 (这是可以设置的)

<span style=color:red;background:yellow;font-size:20px>**(4)持久性durability**</span>

事务的持久性是指**一旦事务提交后或者回滚，事务对数据库的改变应该是永久性的**。 (因为: 提交和回滚,  都会结束事务)

#### 并发可能产生的问题

##### 1.脏写

##### 2.脏读(Dirty read)

<span style=color:red;font-size:20px>一个事务读取到了另外一个事务还没提交的数据:即脏读(读到了脏数据)</span>——**即另一个事务能看到一个事务的修改数据(未提交)**

##### 3.不可重复读(nonrepeatable read)

<span style=color:red;font-size:20px>在同一个事务内，针对同一个数据，前后读取的数据不一样: 即不可重复读</span>——**即另一个事务能看到一个事务的修改数据(提交/未提交)**

##### 4.虚幻读/幻读/幻影读(phantom  read)

<span style=color:red;font-size:20px>在一个事务内，任何一条数据的内容前后读取一致, 但是数据条数前后不一定一致。</span>——**即另一个事务能看到一个事务的添加(删除)数据(已提交-因为未提交时也属于脏读范畴)**

##### 区别

> 1. **脏读、不可重复读**：都是针对**已存在的行**——**行内容问题**
> 2. **幻读**：是针对**行的数量变化**——**行数量问题**
>
> - 幻读若是未提交时属于脏读范畴：只要是 “读到了别人没提交的数据”，不管是 update、insert 还是 delete，统统都叫脏读
> - 可重复读一定—>不可脏读，若是存在脏读则一定—>不可重复读

#### 隔离级别

|               隔离级别               |               隔离效果               |
| :----------------------------------: | :----------------------------------: |
|     READ UNCOMMITTED（读未提交）     |   别人能看到你**未提交**的临时数据   |
| READ COMMITTED（读已提交，**默认**） | 别人只能看到你**commit 之后**的数据  |
|     REPEATABLE READ（可重复读）      | 别人完全看不到你中间修改，直到你提交 |
|        Serializable（串行化）        |         直接锁表，互相看不见         |

| ==隔离级别（均无脏写）==         | ==脏读== | ==不可重复读== | ==虚幻读== |
| :------------------------------- | :------: | :------------: | :--------: |
| **读未提交（read uncommitted）** |    ✅️     |       ✅️        |     ✅️      |
| **读已提交（read committed）**   |  **X**   |       ✅️        |     ✅️      |
| **可重复读（repeatable read）**  |  **X**   |     **X**      |  **✅️-×**   |
| **串行化（serializable）**       |  **X**   |     **X**      |   **X**    |

