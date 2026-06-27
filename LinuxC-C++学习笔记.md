# Linux C/C++学习笔记

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

## 1 形参指针变量修改

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

## 2 通讯录项目实现


