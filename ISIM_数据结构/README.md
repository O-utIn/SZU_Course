
# 数据结构复习提纲
该复习提纲主要针对概念题与计算题，不含代码部分，具体算法实现代码请自行复习


# 1.绪论
## 基本概念和术语
数据分成三个层次：**数据**、**数据元素**、**数据项**：\
1.**数据**是对客观事物的符号表示，在计算机科学中是指所有能输入到计算机中并被计算机程序处理的符合的总称。\
\-数值型数据\
\-非数值型数据\
2.**数据元素**是数据的基本单位，在计算机程序中通常作为一个整体进行考虑和处理。\
\-数据元素又称为元素、结点、记录\
3.**数据对象（数据项**：具有独立含义的最小标识单位，是性质相同的数据元素的集合，是数据的一个子集。

结构是数据元素之间的关系\
\-空间位置关系\
\-相互作用和依赖关系\
**数据结构**是相互之间存在一种或多种特定关系的数据元素的集合\
数据结构的形式定义是一个二元组\
四种结构：\
\-**集合**\
\-**线性结构**\
\-**树形结构**\
\-**图状结构**或**网状结构**

### 数据结构分为逻辑结构和存储结构
结构定义中的“关系”描述的是数据元素之间的逻辑关系，称为**逻辑结构**\
\-线性结构，线性表（表、栈、队列、串等）\
\-非线性结构，树、图等\
数据结构在计算机中的表示（映像）称为数据的**物理结构**，又称**存储结构**。它包括数据元素的表示和关系的表示\
\-顺序存储表示（例如C语言中的一维数组）\
\-链接存储表示（例如C语言中的指针）\
\-索引存储表示\
\-散列存储表示

**数据类型**是与数据结构密切相关的一个概念，用于描述程序操作对象的特性。可以理解为数据类型是一个值的集合和定义在这个值集上的一组操作的总称。
\-如C语言中的整型变量(int)，其值集为某个区间上的整数，定义在其上的操作为+, -, ×, /等\
**抽象数据类型**（Abstract Data Type，简称ADT），是指一个数学模型以及定义在该模型上的一组操作。\
抽象数据类型 = 数据结构 + 定义在此数据结构上的一组操作\
ADT的形式定义，三元组表示:\
	（D，S，P）\
	D是数据对象\
	S是D上的关系集\
	P是对D的基本操作集\
 ADT的示例：\
![image](https://github.com/user-attachments/assets/a1ade538-9c43-47c7-a033-e968fb01a575)

### 算法
**算法**是对特定问题求解步骤的一种描述，是一有限长的操作序列\
算法特性\
**有穷性**：算法在执行有穷步后能结束\
**确定性**：每步定义都是确切的、无歧义的\
**可行性**：算法描述的操作都是可以通过已实现的基本运算经过有限次来实现的\
**输  入**：有0个或多个输入\
**输  出**：有一个或多个输出

算法要求\
**正确性**：满足具体问题的需求\
**可读性**：便于理解和修改\
**健壮性**：当输入数据非法时，也能适当反应\
**效率与低存储需求**\
\-效率高：执行时间少，时间复杂度\
\-空间省：执行中需要的最大存储空间要少，空间复杂度

### 时间复杂度
**时间复杂度**，衡量算法的时间效率，主要依据算法执行所需要的时间\
**事后统计法**：计算算法开始时间与完成时间差值\
**事前统计法**：依据算法选用何种策略及问题的规模n，是常用的方法

时间复杂度是问题规模n的函数f(n)，即：\
    T(n) = O(f(n))\
一般地，时间复杂度用算法中最深层循环内的语句中的原操作的重复执行次数表示

常见的时间复杂度有：\
常量阶、对数阶、线性阶、平方阶、立方阶、指数阶、排列阶\
 O(1) < O(log2n) < O(n) < O(n2) < O(n3) < O(2n) < O(n!)\
一般时间复杂度低于平方阶的算法视为高效率算法\
一般指数阶算法视为不可行算法，例如NP难题、NP完全问题多是指时间复杂度为指数阶或更高阶

时间复杂度并不唯一，因为算法的执行有多种可能的操作顺序\
当可以估算各种操作可能，一般计算**平均时间复杂度**\
如果无法计算平均时间复杂度，则采用**最坏情况下**的时间复杂度，又称**最坏时间复杂度**

**时间复杂度是衡量算法好坏的最重要标准**

### 空间复杂度
空间复杂度指算法执行时，所需要存储空间的量度，它也是问题规模的函数，即：\
　　　S(n) = O(f(n))，除输入和程序之外的额外空间\
一个程序运行需要的存储空间包括三部分：\
程序代码空间\
输入数据空间\
算法执行需要的辅助空间\
空间复杂度只考虑除输入和程序之外的额外空间\
空间复杂度只考虑辅助变量的额外空间

# 2.线性表
线性数据结构的特点\
**同一性**，同一个线性表的数据属同一类数据对象\
![image](https://github.com/user-attachments/assets/c697f3de-d7ee-4fab-97a8-ec27c455b5aa)


**顺序性**，数据之间存在序偶关系
数据顺序性
除第一个元素外，每个数据元素均只有一个直接前驱
除最后一个元素外，每个数据元素均只有一个直接后继(next)
没有直接前驱的元素即第一个数据元素
没有直接后继的元素即最后一个元素
![image](https://github.com/user-attachments/assets/e82734b0-3c02-463a-aa47-8930eea43f14)
![image](https://github.com/user-attachments/assets/46daec47-9ebb-4a75-a62e-7dfcfc612eaa)


## 线性表的顺序表示和实现：顺序表
![image](https://github.com/user-attachments/assets/973e761e-7f70-489f-860a-91a91ab78bcb)
![image](https://github.com/user-attachments/assets/ff3a1ddd-a0fa-4e18-a734-eef35502aa7d)

### 顺序表的优点：
元素位置可用一个简单、直观的公式表示并读写\
元素可以随机存取

### 顺序表的缺点：
在作插入或删除操作时，需要移动大量元素\
因此引入链表，减少操作

## 线性表的链式表示和实现：链表
![image](https://github.com/user-attachments/assets/4847df6e-081e-47c6-800c-b478f9c0ee5d)

单链表插入的时间复杂度主要取决于while循环中的语句频度\
循环频度与在链表中的元素位置查找有关，因此线性链表插入的时间复杂度为O(n)

### 单链表的删除：给出要删除的链表、删除位置
删除是把链表的第i-1元素与第i+1元素之间的第i元素删除\
链表的删除不需要移动元素，只需要修改指针\
单链表删除的时间复杂度主要取决于while循环中的语句频度\
循环频度与在链表中的元素位置查找有关，因此线性链表删除的时间复杂度为O(n)\

### 顺序表与链表的比较（空间）
存储分配的方式\
顺序表的存储空间是静态分配的\
链表的存储空间是动态分配的\
存储密度 = 结点数据本身所占的存储量/结点结构所占的存储总量\
顺序表的存储密度 = 1\
链表的存储密度 < 1

### 顺序表与链表的比较（时间）
存取方式\
顺序表可以随机存取，也可以顺序存取\
链表必须顺序存取\
插入/删除时移动元素个数\
顺序表平均需要移动近一半元素\
链表不需要移动元素，只需要修改指针
![image](https://github.com/user-attachments/assets/7eb276c5-74ab-469a-8a4c-4c4b1ae1c35a)
![image](https://github.com/user-attachments/assets/0fd203e3-04da-4f3f-b549-2c12d9541622)

# 3.栈和队列
## 栈的概念
栈是限定仅在表尾（top）进行插入或删除操作的线性表\
允许插入和删除的一端称为栈顶（top，表尾）\
栈的另一端称为栈底（bottom，表头）\
栈的特点，后进先出（LIFO,last in first out）
![image](https://github.com/user-attachments/assets/1f3e5413-81c3-4c4d-ae3b-e033d47ea0e0)

### 顺序栈
顺序栈是栈的一种顺序实现，是顺序存储结构，利用一组地址连续的存储单元依次存放自栈底到栈顶的数据元素
![image](https://github.com/user-attachments/assets/0d11d9eb-9c35-44be-bc0c-8d6b8e1706ed)
![image](https://github.com/user-attachments/assets/6bb59796-212b-4699-8539-ad1fbcfac2dc)

顺序栈的特性：\
top==0或top==base，表示空栈\
base=NULL表示栈不存在\
入栈，只能从栈顶插入，指针top++\
出栈，只能从栈顶删除，指针top--\
当top>stacksize时，栈满，溢出，又称“上溢”

### 链栈
链栈是栈的链式存储结构，是运算受限的单链表\
其插入和删除操作只能在表头位置上进行\
链栈用没有头结点的单链表，链表头指针表示栈顶指针top
![image](https://github.com/user-attachments/assets/4c886c12-8b75-425c-82fa-5997bebe56ad)
![image](https://github.com/user-attachments/assets/c9b9cf53-187f-48b6-b7bf-0862be9a6368)

**注意栈的后进先出操作，顺序栈不是数组**\
**顺序栈不能执行数组的遍历操作**

![image](https://github.com/user-attachments/assets/0006ea49-1583-4f8c-8b2d-f67d4a05a27f)
![image](https://github.com/user-attachments/assets/86c711b9-8ca8-43f2-b6c8-cb2d35d01d23)
\
\
共享栈的判栈满条件：\
![image](https://github.com/user-attachments/assets/c6bf9b74-6f29-4c33-a0fe-20ce596ff2a1)
![image](https://github.com/user-attachments/assets/ecf45910-8259-4b2a-a6d9-e4a5dc829c6a)


栈是限定操作的线性表，特点后进先出\
栈的出栈可能结果序列、入栈出栈操作序列\
顺序栈，理解top栈顶、base栈底\
插入、删除、取栈顶的操作，top的变化\
判栈空top==base\
链栈，采用头插法，top是一个指针，指向首结点\
共享栈，两个栈共用一个数组空间，两个栈顶相邻则栈满\
![image](https://github.com/user-attachments/assets/6df0ef3a-f9d8-42d8-9250-0d43594a8cdd)

## 队列
**队列特点**：先进先出（FIFO）\
队列的入队序列和出队序列完全相同的，即出队结果是唯一的\
队列同样是操作受限的的线性表\
在队列中，允许插入的一端叫队尾（rear），允许删除的一端称为队头（front）\
队列的存储结构：顺序队列、循环队列、链队列\

![image](https://github.com/user-attachments/assets/23cc74ff-4251-475b-ab2d-8ca07d6e364b)
![image](https://github.com/user-attachments/assets/f6a60341-8a92-46e8-afe6-1957a21c85d6)


![image](https://github.com/user-attachments/assets/e929dab8-6057-4135-819a-f979e4fd8d2b)
![image](https://github.com/user-attachments/assets/d17987c0-3339-4210-b7d4-acd6dcecca18)

![image](https://github.com/user-attachments/assets/a54269da-01e6-4f44-b30c-c739cd6fcea4)

![image](https://github.com/user-attachments/assets/8d9475fd-32ad-4661-a591-57792d35c6ec)
![image](https://github.com/user-attachments/assets/d0b24282-436b-4a9a-ad59-d6068bb4fe28)

![image](https://github.com/user-attachments/assets/18100fa4-45cc-4553-9f9d-db4c6c757bb0)


# 4.串
![image](https://github.com/user-attachments/assets/a6733aed-6874-4047-8064-5f28ee24a99e)
![image](https://github.com/user-attachments/assets/32146ca2-56ac-42bc-b9fd-05c6fdbd24d4)
![image](https://github.com/user-attachments/assets/231e829b-4aa7-4732-83c8-e42a65650aad)

## 串的表现和实现
![image](https://github.com/user-attachments/assets/3a42454e-e512-4b37-88ac-965f671d5d1f)

## 串的模式匹配算法
![image](https://github.com/user-attachments/assets/9bef8f9c-6831-4a5f-9b94-7bd3c28cdd9f)
![image](https://github.com/user-attachments/assets/23bebd62-1b4d-4324-b36a-3efc78af9b57)
穷举法的比较示例\
主串：a b a b c a b c a c b a b\
模式串：a b c a c

![image](https://github.com/user-attachments/assets/06fdc8d5-ae8b-4d92-9cf7-bdddaabae2bd)

![image](https://github.com/user-attachments/assets/7c796d01-5595-41e4-a06d-f1acc67c11be)

![image](https://github.com/user-attachments/assets/131ff62b-3f60-4d88-8447-630bd1fbfca1)

![image](https://github.com/user-attachments/assets/179e2d2d-12f4-42e7-9f5b-32e9c6d94e6b)
![image](https://github.com/user-attachments/assets/be023fbe-8ba1-4455-b40d-18f66a962359)

![image](https://github.com/user-attachments/assets/3c477915-e325-475f-a6b5-52fc2f2963af)
![image](https://github.com/user-attachments/assets/e6363b34-8dab-4527-9190-da1ee15532e6)
![image](https://github.com/user-attachments/assets/a0579d7c-09a0-449b-8ebc-1ef377369ac1)

![image](https://github.com/user-attachments/assets/dbcfa881-f78b-4142-85a7-1421f597e02d)

![image](https://github.com/user-attachments/assets/27aad905-788d-4ed1-8223-4bf8b0ae64e3)


# 6.树
树是有n（n>=0）个结点的有限集合。每个结点都有唯一的直接前驱，但可能有多个直接后继

树的特点
- 如果n=0，称为空树
- 如果n>0,称为非空树
- 树可以递归定义
- 如果n>1，则除根结点以外的其他结点划分为m（m>0）个互不相交的有限集T1，T2，...，Tm，其中每个集合本身又是一棵树，并且称为树的子树SubTree

树结构的术语：
- 结点：包含一个数据元素及若干指向其子树的分支
- 分支：结点之间的连接
- 结点的度：结点拥有的子树数
- 树的度：树中结点度的最大值称为树的度
- 叶结点：度为0的结点[没有子树的结点]
- 分支结点：度不为0的结点[包括根结点]，也称为非终端结点。
- 分支结点包含根结点，除根外的分支结点又称为内部结点

![image](https://github.com/user-attachments/assets/4651d2a2-d48f-4556-b112-f68322f796c9)

其他术语\
层次：根结点为第一层，其孩子为第二层，依此类推\
深度：树中结点的最大层次
![image](https://github.com/user-attachments/assets/017e4bab-b876-4993-bc67-a40c754b6dcd)

森林：互不相交的树的集合。对树中每个结点而言，其子树的集合即为森林
![image](https://github.com/user-attachments/assets/778c630f-b3ec-4ce9-a906-35979f4e4fe2)


![image](https://github.com/user-attachments/assets/5a07c670-0985-4739-a741-7fcfad3d2343)

![image](https://github.com/user-attachments/assets/4115fa56-01a1-49ae-ba26-cc47ae271c6c)
![image](https://github.com/user-attachments/assets/93f73a5b-a58e-41a6-893c-5c84f959840c)

![image](https://github.com/user-attachments/assets/1a5e1870-f176-4968-9e39-1609973a3201)
![image](https://github.com/user-attachments/assets/77fdab8b-8244-4d6e-beb4-f6fddfa03770)
![image](https://github.com/user-attachments/assets/325e719b-b101-4476-82e1-ef258c9ce0e8)

![image](https://github.com/user-attachments/assets/e2ec77b6-ae17-4034-a4ae-731c9d99fc18)
![image](https://github.com/user-attachments/assets/56a8d95a-fa35-45fc-a843-bab7cb5f4aba)


二叉树的形态数量问题属于树的计数问题\
n个结点的不同二叉树形态有![image](https://github.com/user-attachments/assets/24f30070-5ab4-4368-9710-77929ded185f)种

![image](https://github.com/user-attachments/assets/ad317959-26af-470a-9bcc-db1d527c2c21)

![image](https://github.com/user-attachments/assets/60b0976b-f2cb-43ad-a27b-0f883f700b14)
![image](https://github.com/user-attachments/assets/4a4c0811-f59e-442e-99ca-e566268ed814)


树的遍历就是按某种次序访问树中的结点，要求每个结点访问一次且仅访问一次\
一个二叉树由根结点与左子树和右子树组成，设访问根结点用D表示，遍历左、右子树用L、R表示，如果规定先左子树后右子树，则共有三种组合
- DLR [先序遍历]
- LDR [中序遍历]
- LRD [后序遍历]

先序遍历的递归算法\
若二叉树为空，则返回；否则：
- 访问当前结点(D)
- 先序遍历左子树(L)
- 先序遍历右子树(R)

![image](https://github.com/user-attachments/assets/922989c6-10f5-4dcc-961a-044ed593d83a)

先序表遍历结果：ABDEGCF\
第一个输出结点必为根

中序遍历的递归算法\
若二叉树为空，则返回；否则：
- 中序遍历左子树(L)
- 访问当前结点(D)
- 中先序遍历右子树(R)

![image](https://github.com/user-attachments/assets/8b6efb2a-09ac-4da9-b7f1-19854122f609)

中序表遍历结果：DBGEAFC\
根结点A的左右子树分别在A的两边

后序遍历的递归算法\
若二叉树为空，则返回；否则：
- 后序遍历左子树(L)
- 后序遍历右子树(R)
- 访问当前结点(D)
  
![image](https://github.com/user-attachments/assets/7b06fb96-5f1b-4dea-a2c9-a8b1c6e537f5)

后序表遍历结果：DGEBFCA\
最后一个输出结点必为根


二叉树层次遍历，从根结点开始遍历，按层次“自上而下，从左至右”访问树中的各结点。\
层次遍历的实现算法，设置一个队列：\
设T是指向根结点的指针变量，若二叉树为空，则返回；否则，令p=T，p入队，执行以下循环：
- (1)队首元素出队到p；
- (2)访问p所指向的结点； 
- (3)p所指向的结点的左、右子结点依次入队。
- (4)跳转步骤1循环，直到队空为止。
  
![image](https://github.com/user-attachments/assets/b71cdd50-1db3-4a29-baed-2dff4a3306c7)

层次遍历：ABCDEFG


![image](https://github.com/user-attachments/assets/95d846b9-5f4b-4216-b0d4-026b898a0d70)
![image](https://github.com/user-attachments/assets/021a64ed-58a6-4d48-9105-fa9c8af0e9a6)
![image](https://github.com/user-attachments/assets/17cc240e-145e-4857-8c4c-2780dc3036ba)


二叉树构建有以下方法：
- 第1种方法：特定的先序遍历字符串来生成二叉树
- 第2种方法：已知二叉树的顺序存储，利用二叉树性质5生成二叉树
- 第3种方法：已知二叉树的先序+中序或者中序+后序，生成二叉树

二叉树构建第1种方法：特定的先序遍历字符串来生成二叉树\
用特定字符表示空树，例如0。\
二叉树的先序遍历包含空孩子指针遍历，生成特定的先序遍历字符串\
例如ABC00DE0G00F000

![image](https://github.com/user-attachments/assets/52dcc7ea-c92f-44c0-9dcc-e2e529c13883)

二叉树构建第2种方法：已知二叉树的顺序存储，通过二叉树性质5构建二叉树
![image](https://github.com/user-attachments/assets/3d54dba5-28ab-4b69-b9bf-6ed82b4c3140)
![image](https://github.com/user-attachments/assets/3ad94335-7dc8-43c6-851a-3d198a59a1c1)


![image](https://github.com/user-attachments/assets/82b626ef-b900-4fe1-a527-7ce322e4ab4d)

![image](https://github.com/user-attachments/assets/44a8e0aa-a38f-471a-b056-b00af9143a62)


![image](https://github.com/user-attachments/assets/35a916fb-0099-4cdf-b831-75c654d8f843)
![image](https://github.com/user-attachments/assets/aac1c3bc-c7eb-46e1-9c1c-af9c1e00b02a)


![image](https://github.com/user-attachments/assets/9779d2d2-08b3-44ac-b9c1-4d5c6881622b)
![image](https://github.com/user-attachments/assets/93b33930-df61-43b6-8bf5-f8c4d892c144)
![image](https://github.com/user-attachments/assets/7052ba3d-88e2-4401-a877-f5297997297f)
![image](https://github.com/user-attachments/assets/5a08d772-1d22-400f-aa95-32e79d0c80cd)

![image](https://github.com/user-attachments/assets/08ae73d0-e4a5-4bc5-b0d5-1d32e01e44f3)
![image](https://github.com/user-attachments/assets/45403747-318b-4d07-8108-2402122d9f4d)
![image](https://github.com/user-attachments/assets/873564ca-37dd-41f8-b9f2-77827a6464af)

![image](https://github.com/user-attachments/assets/d122cfe3-e093-4696-a2c0-db70c97bcb22)
![image](https://github.com/user-attachments/assets/55a8dc60-4dd4-4c67-a165-77543c7aea63)
![image](https://github.com/user-attachments/assets/d50c9de9-a3bf-424c-a086-6d6830e35b7a)


![image](https://github.com/user-attachments/assets/3ced489b-b945-4a64-bc38-a2ddfe0b39cb)
![image](https://github.com/user-attachments/assets/ccc200da-846b-4784-9067-07f9dce4d7da)


## 树和森林
![image](https://github.com/user-attachments/assets/c861f923-bb6d-41ac-881c-10322577f0b6)

![image](https://github.com/user-attachments/assets/b70087e0-ae81-4079-9c3e-45a9a48d63ba)
![image](https://github.com/user-attachments/assets/e376b190-f86a-46a0-86b7-d912a8813656)
![image](https://github.com/user-attachments/assets/5affc43b-5338-4a13-b2c0-3077355f724b)
![image](https://github.com/user-attachments/assets/b198807b-eac0-4511-b354-5234369de3d2)

![image](https://github.com/user-attachments/assets/a272cdd1-d390-4419-bc8a-a4a6b988aef5)
![image](https://github.com/user-attachments/assets/4f38981f-3403-4900-b17c-1270ab065445)

![image](https://github.com/user-attachments/assets/81dfdda6-ed62-4ccd-9c37-da90ef802429)
![image](https://github.com/user-attachments/assets/08ae5efc-1c7c-42d3-9457-85e3fc42b2b2)

![image](https://github.com/user-attachments/assets/d5df4d56-79dc-419f-ae5b-0a2cb0b4a57e)
![image](https://github.com/user-attachments/assets/8b04cb00-591a-468a-89d4-a3e2ff380871)
![image](https://github.com/user-attachments/assets/e9c5d913-21cb-4008-b880-91b3a2cd7220)
![image](https://github.com/user-attachments/assets/425f2caa-fe15-4456-a010-5344896ccbc8)

![image](https://github.com/user-attachments/assets/686335cf-2bb4-41e7-827d-d5da732fada6)
![image](https://github.com/user-attachments/assets/fb15c2e9-38ca-420b-8269-1440bf02b32c)

![image](https://github.com/user-attachments/assets/debc1b3d-e236-4eb3-ae02-823c0f676f6c)
![image](https://github.com/user-attachments/assets/397d3824-2545-4e96-bc59-9535cea2ac88)
![image](https://github.com/user-attachments/assets/ea47389c-7c48-4bc3-8c85-beb87c18de94)

![image](https://github.com/user-attachments/assets/890ccf27-823c-4f2d-b7c6-d249168d54fd)
![image](https://github.com/user-attachments/assets/beb3613e-e3d8-401d-89df-3f9278f616fc)

![image](https://github.com/user-attachments/assets/a8d6a9cd-44d7-4d2e-adc0-5c70c871d8a2)
![image](https://github.com/user-attachments/assets/d0845a95-71bb-4efd-b353-a8a515632dae)

![image](https://github.com/user-attachments/assets/fce5c5db-01f1-4051-b5c0-be2650ea7131)
![image](https://github.com/user-attachments/assets/afe54b66-df92-4aa4-93b9-40557c16c4e5)

![image](https://github.com/user-attachments/assets/2def40ae-8cc4-430d-863e-de79a51694ec)
![image](https://github.com/user-attachments/assets/2ebe53d7-3333-442f-996c-333c633dc9db)


# 7.图
图(Graph)是一种比线性表和树更为复杂的数据结构。\
线性结构：是研究数据元素之间的一对一关系，数据元素之间有明显顺序管理\
树结构：是研究数据元素之间的一对多的关系，数据元素之间有明显的层次关系\
图结构：是研究数据元素之间的多对多的关系。任意两个元素之间可能存在关系，即结点之间的关系可以是任意的，图中任意元素之间都可能相关。

![image](https://github.com/user-attachments/assets/60a6cd04-5f83-484e-a663-1b754a42d64b)





