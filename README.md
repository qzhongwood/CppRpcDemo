# CppRpcDemo
C++版本的 RPC demo.

开发环境：
======
vs2005，windows7


主要功能：
======
- 1.  invoke线程安全，client发送请求方法线程安全
- 2.  所有动态分配对象使用线程安全的智能指针
- 3.  服务器支持多线程
- 4.  使用windows的IOCP作为异步的I/O模型，client和server均为异步I/O
- 5.  支持同步异步2种方式发送请求



如何使用：
======
- Run as server： rpcdemo.exe -p port
  - port 为server监听端口
- Run as client： rpcdemo.exe -p port -h ip -n numRequest -a
  - port为server监听端口
  - ip为server ip地址
  - numRequest为一个client提交的请求数量
  - "-a"该option为异步发送请求， 无该option则为同步发送
