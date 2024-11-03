#include <memory>
#include <mutex>
#include <iostream>

template <typename T>
class singleton
{
public:
    ~singleton(){
        std::cout<<"sigleton is destruct\n";
    }

private:
    static std::shared_ptr<T> getInstance(){
        static std::once_flag s_flag;
        //内部实现加锁
        std::call_once(s_flag,[&](){
            _instance=std::shared_ptr<T>(new T);
        });
        return _instance;
    }
protected:
    singleton()=default;
    singleton(singleton &&) = delete;//移动构造
    singleton(const singleton<T> &) = delete;//拷贝构造
    singleton &operator=(singleton &&) = delete;//移动赋值
    singleton &operator=(const singleton &) = delete;//const类型移动赋值

    static std::shared_ptr<T> _instance;

};

template<typename T>
std::shared_ptr<T> singleton<T>::_instance=nullptr;