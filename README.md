# sproto-c
sproto c binding  a1406@163.com

解包时候的内存分配太蛋疼了，先这样吧。如果不需要和lua那些兼容，可以在encode的时候把size保存进去。但是要和lua兼容的话，没想好怎么分配内存，总不能分配固定大小，不够了就realloc吧。或者难道解包两次，第一次算算size分配内存，第二次再拷贝？谁有好的法子告诉我，xiexie。

想来想去，做C BINDING只能解包两次，这样太傻了，而且做起来很麻烦。
我试试直接去改sproto吧

差不多了，只差内存释放了。

内存释放也搞定了，还剩下就是安全性检查了。
对了，还没有做字段ID的对应，也就是说，每个字段必须都存在，而且次序必须保持一致. 做起来有点麻烦，要在field里面记录offset

在test_protobuf里面是sproto 和protobufc的对比，在没有做pack压缩的情况下

        size  time
protobuf 159: 2652913
sproto   384: 2491097

在pack的情况下

         size  time
protobuf 159: 2650560
sproto   188: 5091389


结论就是，protobuf比sproto好


