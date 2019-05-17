#if !defined(BUFFERIMPL_H_)
#define BUFFERIMPL_H_

#include <iostream>
#include <stdio.h>

#include "Buffer.h"

template <typename T>
class BufferImpl : public Buffer
{
private:
    
    int length_, width_, height_;
    T **data_ptr_;

public:
    BufferImpl();
    ~BufferImpl();

    void* GetValue(int, int, int);
    bool Load(char*, int, int, int);
    bool Save(char*, int, int, int);
};

template<class T>
BufferImpl<T>::BufferImpl()
{
    length_ = 0;
    width_ = 0;
    height_ = 0;
    data_ptr_ = nullptr;
}

template<class T>
BufferImpl<T>::~BufferImpl()
{
    if (data_ptr_) {
        for (size_t i = 0; i < height_; i++)
        {
            if (data_ptr_[i]) {
                delete[] data_ptr_[i];
            }
        }
        delete[] data_ptr_;
    }
    data_ptr_ = nullptr;
}

template<class T>
void* BufferImpl<T>::GetValue(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0)
    {
        printf("BufferImpl : (x, y, z)->(%d, %d, %d) is illegal\n", x, y, z);
        return nullptr;
    }
    if (data_ptr_)
    {
        return &data_ptr_[z][y*length_+x];
    } else {
        printf("BufferImpl : data_ptr_ is nullptr\n");
        return nullptr;
    }
}

template<class T>
bool BufferImpl<T>::Load(char *filepath, int l, int w, int h)
{
    //检验参数
    if (l < 1 || w < 1 || h < 1 || !filepath) {
        return false;
    }
    FILE *datafile = fopen(filepath, "r+b");
    if (!datafile) {
        printf("LoadData : open file failed~~~\n");
        return false;
    }
    //判别大小是否一致，否则就要重新构造数组
    if (length_ != l || width_ != w || height_ != h) {
        if (data_ptr_) {
            for (size_t i = 0; i < height_; i++)
            {
                if (data_ptr_[i]) {
                    delete[] data_ptr_[i];
                }
            }
            delete[] data_ptr_;
        }
        length_ = l; width_ = w; height_ = h;
        data_ptr_ = new T*[height_];
        for (size_t j = 0; j < height_; j++)
        {
            data_ptr_[j] = new T[length_*width_];
        }
    }
    //读入数据
    for (int k = 0; k < height_; k++)
    {
        fread(data_ptr_[k], sizeof(T), length_ * width_, datafile);
    }
    fclose(datafile);
    return true;
}

template<class T>
bool BufferImpl<T>::Save(char *filepath, int l, int w, int h)
{
    //检验参数
    if (l < 1 || w < 1 || h < 1 || !filepath) {
        return false;
    }
    //检查大小是否一致
    if (length_ != l || width_ != w || height_ != h) {
        return false;
    }
    FILE *datafile = fopen(filepath, "r+b");
    if (!datafile) {
        printf("LoadData : open file failed~~~\n");
        return false;
    }
    //写入数据
    for (int k = 0; k < height_; k++)
    {
        fwrite(data_ptr_[k], sizeof(T), length_ * width_, datafile);
    }
    fclose(datafile);
    return true;
}

#endif // BUFFERIMPL_H_
