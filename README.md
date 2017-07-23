# CppRpcDemo
C++版本的 RPC demo.

开发环境：
======
vs2005，windows7


主要功能：
======
1.  invoke线程安全，client发送请求方法线程安全
2.  所有动态分配对象使用线程安全的智能指针
3   服务器支持多线程
4.  使用windows的IOCP作为异步的I/O模型，client和server均为异步I/O



如何使用：
======
Run as server： rpcdemo.exe -p port
Run as client： rpcdemo.exe -p port -h ip -n numRequest
numRequest为一个client提交的请求数量
