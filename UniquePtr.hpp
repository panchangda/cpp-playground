
// #define DebugFuncSig

#ifdef DebugFuncSig
    #define PUTS_FUNCSIG puts(__FUNCSIG__);
#else
    #define PUTS_FUNCSIG ;
#endif

/**
 * 
 * FAQs:
 * 1: Why explicit?
 * 2: Why = delete?
 * 3: Why noexcept?
 * 4. Why != ?
 * 5. Why destruct in move assignment?
 *
*/

template <class T>
struct DefaultDeleter
{
    void operator()(T *p) const
    {
        delete p;
    }
};

template <class T>
struct DefaultDeleter<T[]>
{
    void operator()(T *p) const
    {
        delete[] p;
    }
};

template <class T, class U>
T exchange(T &dst, U &&val) { // 同标准库的 std::exchange
    T tmp = std::move(dst);
    dst = std::forward<U>(val);
    return tmp;
}

template <class T, class Deleter = DefaultDeleter<T>>
class UniquePtr
{
private:
    T* m_p;
    
    template <class U, class UDeleter>
    friend struct UniquePtr;
    
public:
    UniquePtr()
    {
        m_p = nullptr;
    }

    /** 1. Why Explicit? */
    explicit UniquePtr(T* p)
    {
        m_p = p;
        PUTS_FUNCSIG
    }

    // https://github.com/parallel101/stl1weekend/blob/main/UniquePtr.hpp
    template <class U, class UDeleter, class = std::enable_if_t<std::is_convertible_v<U *, T *>>> // 没有 C++20 的写法
    // template <class U, class UDeleter> requires (std::convertible_to<U *, T *>) // 有 C++20 的写法
    UniquePtr(UniquePtr<U, UDeleter> &&that) {  // 从子类型U的智能指针转换到T类型的智能指针
        m_p = exchange(that.m_p, nullptr);
    }

    

    /** 2. Why = delete? */
    // Delete Copy Construction&Assignment
    UniquePtr(const UniquePtr<T>& that) = delete;
    UniquePtr<T>& operator=(const UniquePtr<T>& that) = delete;

    
    /** 3. Why noexcept? */
    UniquePtr(UniquePtr<T>&& that) noexcept
    {
        m_p = exchange(that.m_p, nullptr);
        PUTS_FUNCSIG
    }

    // Move Assignment
    UniquePtr<T>& operator=(UniquePtr<T>&& that) noexcept
    {
        if (this != &that) /** 4. Why != ?*/
        {
            if (m_p)
            {
                Deleter{}(m_p); /** 5. Why destruct in move assignment?*/
            }
            m_p = that.m_p;
            that.m_p = nullptr;
            // m_p = std::exchange(that.m_p, nullptr);
        }
        PUTS_FUNCSIG
        return *this;
    }

    ~UniquePtr()
    {
        if (m_p)
        {
            Deleter{}(m_p);
        }
        PUTS_FUNCSIG
    }

    // get, operator overloads... for convenience 
    T* get() const
    {
        return m_p;
    }

    T& operator*() const
    {
        return *m_p;
    }
    
    T* operator->() const // Recursively Call
    {
        return m_p;
    }
};

template<class T, class Deleter>
struct UniquePtr<T[], Deleter> : UniquePtr<T, Deleter> {};

template<class T, class ...Args>
UniquePtr<T> MakeUnique(Args&& ...args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
