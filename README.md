# File System

```cpp
//定义新的索引数量
#define NDIRECT 11
#define NINDIRECT (BSIZE / sizeof(uint))
#define NINDIRECT2 (NINDIRECT*NINDIRECT)
#define MAXFILE (NDIRECT + NINDIRECT + NINDIRECT2)
```