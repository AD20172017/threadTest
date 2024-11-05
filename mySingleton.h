#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <memory>
#include <mutex>
#include <iostream>
//模板类的子类必须显示调用模板类才能使用其成员函数
template <typename T>
class singleton
{
protected:
    singleton() = default;
    // singleton(singleton &&) = delete;//移动构造
    singleton(const singleton<T> &) = delete; // 拷贝构造
    // singleton &operator=(singleton &&) = delete;//移动赋值
    singleton &operator=(const singleton &) = delete; // const类型移动赋值

    static std::shared_ptr<T> _instance;

public:
    ~singleton()
    {
        std::cout << "singleton is destruct\n";
    }

    static std::shared_ptr<T> getInstance()
    {
        static std::once_flag s_flag;
        // 内部实现加锁
        std::call_once(s_flag, [&]()
                       { _instance = std::shared_ptr<T>(new T); });
        return _instance;
    }

private:
};

template <typename T>
std::shared_ptr<T> singleton<T>::_instance = nullptr;
#endif // !_SINGLETON_H_