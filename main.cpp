#include "UniquePtr.h"
#include "Function.hpp"

#include <iostream>

/**
 * 
 * FAQs:
 * 1: Why explicit?
 * 2: Why = delete?
 * 3: Why noexcept?
    4. Why != ?
*/

template <class T>
class UniquePtr
{
private:
    T *m_p;

public:
    UniquePtr(): m_p(nullptr)
    {
        puts(__FUNCSIG__);
    }
    /** 1. Why Explicit? */ 
    explicit UniquePtr(T* p)
    {
        m_p = p;
        puts(__FUNCSIG__);
    }
    /** 2. Why = delete? */
    // Delete Copy Construction&Assignment
    UniquePtr(const UniquePtr<T>& that) = delete;
    UniquePtr<T>& operator=(const UniquePtr<T>& that) = delete;
    /** 3. Why noexcept? */
    UniquePtr(UniquePtr<T> &&that) noexcept
    {
        m_p = std::exchange(that.m_p, nullptr);
        puts(__FUNCSIG__);
    }
    // Move Assignment
    UniquePtr<T>& operator=(UniquePtr<T>&& that) noexcept
    {
        if( this!= &that) /** 4. Why != */
        {
            std::exchange(that.m_p, nullptr);
        }
        puts(__FUNCSIG__);
    }
    ~UniquePtr()
    {
        puts(__FUNCSIG__);
    }
    
};

void foo1(UniquePtr<int>u_ptr)
{
    
}

void foo2(UniquePtr<int>u_ptr)
{
    
}
int main()
{
    // 1. Why explicit?
    // int* p = new int(5);
    // UniquePtr<int> u_ptr(p);
    // foo1(p);

    // 2. Why = delete?
    // auto a = UniquePtr<int>(new int(4));
    // UniquePtr<int>b;
    // b = a;
    // auto c = UniquePtr<int>(a);

    
    puts("1");
    
    
    return 0;
}