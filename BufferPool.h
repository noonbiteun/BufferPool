#if !defined(BUFFERPOOL_H_)
#define BUFFERPOOL_H_

#include <iostream>
#include <string.h>
#include <time.h>

#include "Buffer.h"

//打包buffer，加入标识和插入时间
struct BufferPacking
{
    char *identity = nullptr;
    Buffer *buffer = nullptr;
    struct timespec add_time;
};

class BufferPool
{
private:
    const int kPoolSize_;
    BufferPacking **container_ptr_; //容器
    
public:
    BufferPool(const int);
    ~BufferPool();

    Buffer* PopBuffer(char*);
    void AddBuffer(char*, Buffer*);
};

BufferPool::BufferPool(const int size) : kPoolSize_(size)
{
    container_ptr_ = new BufferPacking*[kPoolSize_];
    for (size_t i = 0; i < kPoolSize_; i++)
    {
        container_ptr_[i] = new BufferPacking();
    }
}

BufferPool::~BufferPool()
{
    if (container_ptr_) {
        for (size_t i = 0; i < kPoolSize_; i++)
        {
            if (container_ptr_[i]) {
                delete container_ptr_[i]->buffer;
                delete container_ptr_[i];
            }
        }
        delete[] container_ptr_;
    }
}

void BufferPool::AddBuffer(char *identity, Buffer *buffer)
{
    if (identity && buffer) {
        for (size_t i = 0; i < kPoolSize_; i++)
        {
            //优先插入标识相同的包装器中
            if (container_ptr_[i]->identity && strcmp(container_ptr_[i]->identity, identity) == 0) {
                delete container_ptr_[i]->buffer; //删除buffer
                container_ptr_[i]->buffer = buffer;
                clock_gettime(CLOCK_REALTIME, &container_ptr_[i]->add_time);
                return;
            }
        }
        for (size_t j = 0; j < kPoolSize_; j++)
        {
            //其次插入空的包装器
            if (container_ptr_[j]->buffer == nullptr) {
                container_ptr_[j]->identity = identity;
                container_ptr_[j]->buffer = buffer;
                clock_gettime(CLOCK_REALTIME, &container_ptr_[j]->add_time);
                return;
            }
        }
        long earliest_add_time = container_ptr_[0]->add_time.tv_nsec;
        int index = 0;
        for (size_t k = 1; k < kPoolSize_; k++)
        {
            //最后插入时间最早的包装器
            if (earliest_add_time > container_ptr_[k]->add_time.tv_nsec) {
                earliest_add_time = container_ptr_[k]->add_time.tv_nsec;
                index = k;
            }
        }
        container_ptr_[index]->identity = identity;
        delete container_ptr_[index]->buffer;//删除buffer
        container_ptr_[index]->buffer = buffer;
        clock_gettime(CLOCK_REALTIME, &container_ptr_[index]->add_time);
    }
}

Buffer* BufferPool::PopBuffer(char *identity)
{
    Buffer *buffer = nullptr;
    if (!identity) {
        printf("BufferPool : identity is nullptr\n");
        return buffer;
    }
    if (!container_ptr_) {
        printf("BufferPool : container is empty\n");
        return buffer;
    }
    for (size_t i = 0; i < kPoolSize_; i++)
    {
        if (strcmp(container_ptr_[i]->identity, identity) == 0) {
            buffer = container_ptr_[i]->buffer;
            container_ptr_[i]->buffer = nullptr;
            break;
        }
    }
    return buffer;
}

#endif // BUFFERPOOL_H_
