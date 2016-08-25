# sproto-c
sproto c binding，not finished

解包时候的内存分配太蛋疼了，先这样吧。如果不需要和lua那些兼容，可以在encode的时候把size保存进去。但是要和lua兼容的话，没想好怎么分配内存，总不能分配固定大小，不够了就realloc吧。谁有好的法子告诉我，xiexie。

