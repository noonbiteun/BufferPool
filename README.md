# BufferPool

**适用于多种数据类型共存的缓存池**

### 适用场景
应对IO密集型任务时，可以采用缓存区```Delay Rewrite```的方法（即延迟覆盖缓存区）将其放入缓存池中，等到缓存池满时重写最早放入的缓存区。
采用以上策略，配上合适的缓存池大小，可以有效地减少对同一块数据的反复加载

### 缓存池特性：多数据类型共存

基于模板实现，可以实现同一个缓存池存储多种不同数据类型的缓存区，在大型程序以及多IO密集任务处理中方便对所有的缓存区进行统一管理，共享同一个缓存池的突出表现方面在于对缓存区存储的弹性拓展;

### 关键代码说明

**Buffer接口**

```c++
/*
** buffer接口
*/
#if !defined(BUFFER_H_)
#define BUFFER_H_

class Buffer
{
public:
    virtual ~Buffer() = default;

    virtual void* GetValue(int, int, int) = 0;
    virtual bool Load(char*, int, int, int) = 0;
    virtual bool Save(char*, int, int, int) = 0;
};

#endif // BUFFER_H_
```

```void* GetValue(int, int, int)```函数返回的是```viod*```类型，目的是为了应对模板实现返回不同类型的值;

正确使用方法：```*((T*)ptr->GetValue(x, y, z))```将```viod```指针转换成对应数据类型```T```的指针，再取值即可

