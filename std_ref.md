# memo

* std::async, std::bind, std::thread 接受第一个参数为callable指针时，剩余参数为callable接受的参数。但是不管callable形参定义是否为引用，传递都时by value，需要使用std::ref传递引用。

* quick_exit()? End the program by calling quick_exit(), which was introduced exactly for this reason to end a program without calling the destructors for global and static objects (see Section 5.8.2, page 162).