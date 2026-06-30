# Linux C/C++学习笔记

虚拟机用户名lhy，密码123321

## 一. Linux基本操作

```linux
cd ../    # 返回上一级
ls ../    # 查看上一级目录的文件


mkdir lhy.sh    # 创建文件
touch lhy.sh    # 创建文件
vim lhy.sh    # 若有文件：则编辑；若无文件，则创建后再编辑



权限：
ls -l     #查看当前目录下，文件的权限
chmod +x ping.sh    # 给.sh文件可执行权限，有该权限则执行上一条后会显示绿色   
```

## 二. Shell脚本编程

### 1 shell 简介

### 2 shell 编程结构

```shell
# 实现功能：测试连接所有虚拟机
#|/bin/bash
头文件
for i in [1...254]; do
    ping -c 2 -i 0.5 192.168.199.$i
    if [ $? -eq 0]; then
        echo "192.168.199.$i is up"
    else 
        echo "192.168.199.$i is down"
    fi
done


# 为了不打印出ping的每一条无效信息，需要修改
    ping -c 2 -i 0.5 192.168.199.$i
    ping -c 2 -i 0.5 192.168.199.$>/dev/null
```

执行shell脚本的cmd：

“./ping.sh” 或者 如果没有头文件则“/bin/bash ./ping.sh”

# 编程技术点解析

### 开始 状态机实现文件单词统计



## 一 通讯录项目

### 1 形参指针变量修改

```c
原代码
int person_insert1(struct person *people, struct person *person){
    if (person == NULL) return -1;
    LIST_INSERT(person, people);
    return 0;
}
改后
int person_insert2(struct person **ppeople, struct person *person){
    if (person == NULL) return -1;
    LIST_INSERT(person, *ppeople);
    return 0;
}
调用
struct person *head = null;
struct person lhy;
person_insert2(head, &lhy)
```

使用方法1会造成的问题：

        在main中，把实参传入方法1的*people后，方法1的代码只修改了形参的值，然后people会被销毁，这样main中的实参值仍然是原来的值，数据丢失。

正确的方法（方法2）：

        使用双指针的方法，把输入的形参变为指针的指针

### 2 通讯录项目实现

#### 1️⃣实现思路——分层设计

需要实现一个通讯录，主要逻辑就是实现一个链表，链表中的每个节点是一个二元组，然后能够实现对链表的插入，删除，查找和遍历。但是实现过程需要分层设计：数据层、接口层、业务层。

    分层设计的好处是，当数据层的链表需要换成其他算法时，只要修改数据层即可，接口层，业务层都无需修改。复用性很高。

#### 2️⃣实现代码重写

##### 底层操作层

```c
// 底层数据层entity，构建的是双向列表
#define LIST_INSERT(item, list) do{\
    item -> previous = NULL; \  //先将待插入的pre置空
    item -> next = list; \ // 然后把待插入的指向链表头节点（即指向A节点）
    list = item; \    // 然后把item赋值给list，使得list更新新节点
}while(0)

#define LIST_REMOVE(item, list) do{
    // 如果item有前驱节点，那么让前驱节点跳过待删除节点
    if(item -> previous){item -> previous -> next = item -> next;}
    // 如果item有后继节点，那么让后继节点跳过待删除节点
    if(item -> next){item -> next -> previous = item -> previous;}
    // 如果要删除的是头结点，更新头指针即可
    if(list == item){list = item -> next;}
    // 最后，清空被删除节点的指针
    item->previous = item->next = NULL;
}while(0)


// 定义联系人
struct person{
    char name[NAME_LENGTH];
    char phone_number[PHONE_NUMBER_LENGTH];
    // 结构体内套用自身，形成链式结构 ***📌重点
    struct person *next;
    struct person *pervious;
}
// 定义通讯录
struct contactss{
    struct person *people;
    int count;
}
```

**注意：**

（1）删除操作时，没有释放掉item的内存。但是释放操作没有在宏中进行，原因如下：

        1：违反单一职责原则

        2：无法在外部对变量进行额外处理

        3：在外部调用时，外部的变量会出现野指针(指向已被释放或无效的内存地址)。

##### 接口层

```
// 接口层interface
```





##### 业务层



##### 主函数



## 二 高并发技术方案——锁

##### 互斥锁（Mutex）

线程获取锁失败的时候，操作系统会将其挂起，进入阻塞状态，放入等待队列中，不消耗CPU资源；如果当前锁被释放，则由内核kernel唤醒等待线程。

```
线程想进入临界区：
  1. 尝试获取锁
  2. 如果锁被占用 → 线程进入睡眠（让出CPU）
  3. 等待被唤醒
  4. 获得锁 → 执行临界区代码
  5. 释放锁 → 唤醒等待的线程
```

* **原理**：如果厕所（临界区）有人，你就去**睡觉**（线程休眠），等别人出来叫醒你。

* **优点**：不浪费CPU资源（睡觉不耗电）。

* **缺点**：叫醒（上下文切换）很耗时，大约需要几千个CPU周期。

* **场景**：**复杂操作**。比如你的联系人程序里，`malloc`分配内存、或者删除节点后要`free`，这些操作很慢，用互斥锁最合适。

##### 

##### 自旋锁（Spinlock）

线程获取锁失败时，不休眠，而是循环不断检查锁状态（类似进入while(1)）,直到获取成功。

```
线程想进入临界区：
  1. 尝试获取锁
  2. 如果锁被占用 → 不停循环检查（忙等）
  3. 直到锁被释放
  4. 获得锁 → 执行临界区代码
  5. 释放锁
```

* **原理**：如果微波炉（临界区）被人占用，你就站在旁边**死死盯着**（CPU忙等），每秒问一万次“好了没？”

* **优点**：没有叫醒开销，响应极快（微秒级）。

* **缺点**：盯着的时候啥也干不了，白白耗电（占满CPU）。

* **场景**：**极短操作**。比如仅仅修改一个标志位（`flag = 1`）。在Linux内核中，如果临界区只有几条指令，就用自旋锁。

##### 

##### 原子操作（Atomic Operation）

由CPU硬件保证（汇编语言编写），一条指令要么完全执行，要么完全不执行，中间不会被中断。

```
由硬件保证操作不可分割：
┌─────────────────────────────────────────────┐
│ count++ 实际上分三步：                      │
│   1. 读取 count 到寄存器                    │
│   2. 寄存器 +1                             │
│   3. 写回 count                            │
│                                             │
│ 原子操作：三步一次完成，中间不被中断！     │
└─────────────────────────────────────────────┘
```

* **原理**：不需要“锁”这个机制。CPU硬件保证，**读-改-写**这三步操作**一气呵成**，中间不允许任何人插队。

* **优点**：**最快**！没有锁机制的开销。

* **缺点**：只能处理简单的数学运算（如 `count++`），没法处理复杂的业务逻辑。

* **场景**：**计数器**。比如统计网站访问量，直接用 `atomic_fetch_add`。

##### 

##### CAS（compare and swap）

* **原理**：不锁门，大家直接冲进去改数据。改之前先看一眼旧值，如果旧值没变，就改；如果变了，就**重试**（重新读、重新算、再试一次）。

* **本质**：这是实现**无锁编程**的基石，可以看作是“原子操作”的升级版。

* **场景**：**高性能容器**。比如Java的`ConcurrentHashMap`、C++的`std::atomic`底层大量使用CAS。
