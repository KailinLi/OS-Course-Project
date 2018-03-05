# 文件系统简介

采用了申请一个$64M$的文件($2^{26}$)，进行模拟控制。

![OS](http://oklhb00qa.bkt.clouddn.com/OS.png)

## 空间划分

空间总计$2^{26}$，使用`uint16_t`进行访问，即16bits，故每块空间$2^{10}$，为1K。

其中，使用位示图表示i_node和磁盘空间，一个i_node占32Bytes。

故最多$2^{16}$个文件和$2^{16}$个i节点，使用位示图，分别使用$2^{13}$即8k空间。

i节点需要$32\times 2^{16} = 2^{21}$，即2M空间。即$2^{11}$个块。

## 异常捕获

大量if语句保证错误不传递。

## 缓存机制

因为位示图和 i_node 访问十分频繁，将这两项连同用户信息，初始化时一并读入内存（cache）

## 仿Linux环境测试

* `make` `make clean`...

* `init`  (without dev first)

* `clean`

* `ls (-a -l)` (test more) `/ () /etc etc`

* `cd` (/ () .. .) (worry input)

* `mkdir` `rmdir` (link number) 递归

* `touch` `rm` (duplicate)

* `cat` `echo` `> >>`

* `vim`

* `chmod` write

* `adduser` `userdel` (read file) (home/kailin) (hostname)

* `su` (root) ($ # change)

* `ln`

* `mv`
