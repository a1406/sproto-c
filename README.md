# sproto-c
sproto c binding，not finished

解包时候的内存分配太蛋疼了，先这样吧。如果不需要和lua那些兼容，可以在encode的时候把size保存进去。但是要和lua兼容的话，没想好怎么分配内存，总不能分配固定大小，不够了就realloc吧。或者难道解包两次，第一次算算size分配内存，第二次再拷贝？谁有好的法子告诉我，xiexie。

想来想去，做C BINDING只能解包两次，这样太傻了，而且做起来很麻烦。
我试试直接去改sproto吧

