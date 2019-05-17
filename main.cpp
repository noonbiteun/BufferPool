#include <iostream>
#include <stdio.h>
#include "BufferImpl.h"
#include "BufferPool.h"

int main(int argc, char const *argv[])
{
    BufferPool pool(3);
    //数据路径（构造测试数据请使用：https://github.com/noonbiteun/VirtualData-simple 中的数据构造Utils）
    char filepath[5][20] = {"./data/0000.dat", "./data/0001.dat", "./data/0002.dat", "./data/0003.dat", "./data/0004.dat"};
    Buffer *ptr;
    
    for (size_t i = 0; i < 3; i++)
    {
        ptr = new BufferImpl<unsigned char>();
        //按照路径，以及长宽高参数来载入数据
        ptr->Load(filepath[i], 1200, 1000, 800);
        printf("load finish\n");
        pool.AddBuffer(filepath[i], ptr);
        printf("add finish\n");
    }
    ptr = pool.PopBuffer(filepath[1]);
    if (ptr)
    {
        for (size_t x = 0; x < 1200; x++)
        {
            for (size_t y = 0; y < 1000; y++)
            {
                for (size_t z = 0; z < 800; z++)
                {
                    printf("(%d, %d, %d) value is %d\n", x, y, z, *((unsigned char *)ptr->GetValue(x, y, z)));
                }
            }
        }
    } else {
        printf("pop null\n");
    }

    delete ptr;
    return 0;
}
