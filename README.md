# minico协程库
<br>
1、介绍：<br>
	minico协程库是轻量，c++11实现的**对称**协程库。<br>
	其中上下文切换封装的是glibc的**ucontext**。<br>
<br>
<br>
2、使用：<br>
	tinyco只有两个接口，非常容易使用，分别是co_start(func)用于运行一个新的协程，co_yield()用于暂停当前协程。<br>
	具体使用见src/main.cc。<br>
<br>
<br>
3、编译：<br>
	进入src文件夹make即可,因为有main.cc示例程序，所以make出来的为可执行文件。<br>
<br>


![image-20210101232928960](readme\image-20210101232928960.png)