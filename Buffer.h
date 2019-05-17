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
