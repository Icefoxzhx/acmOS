### Problem 1

(1) 

0000000080000000 T _entry

000000008000095c T start

000000008000001c T main

(2)

bt



### Problem 2

执行kernel.img启动的。而kernel.img就是编译脚本最终生成的目标文件。



### Problem 3

在⽤户态的printf经过层层调用最终需要通过系统调⽤进⼊内核态输出函数。

而内核输出函数printk直接输出⽬标函数。

另外printk是行驱动的。

printk允许通过指定一个标志来设置优先级。  