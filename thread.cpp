#include<iostream>
#include <thread>
#include"boost/format.hpp"
#include<vector>
#include<functional>
#include<mutex>
#include<condition_variable>
#include<random>
#include <future>
#include"spinlock_mutex.hpp"
#include"scoped_thread.hpp"
#include"compete_test.hpp"
// #include <unistd.h>
bool mutex_flag=false;
std::mutex mut_;
std::condition_variable data_cond;




static int x=0;
void thread_finish(){
    boost::format fmt{"test over and x is %1%***********************\n"};
    fmt%x++;
    std::cout << fmt.str() ;
}
void thread_do_something(int& i){
    // if(mutex_flag)std::lock_guard<std::mutex> mygurad(mut_);
    //std::lock_guard<std::mutex> 出if就结束了,必须严格遵守语句块
    std::unique_lock<std::mutex> lock(mut_);
    if(!mutex_flag)lock.unlock();
    compete_test c(x,mut_);
    //有x输出为-1时发生冲突
    boost::format fmt{"This is %1% st thread, id is %3% and x is %2%\n"};
    fmt%i++%c._x++%std::this_thread::get_id();
    std::cout<<fmt.str();
    data_cond.notify_one();
      
}
void test(){
    scoped_thread finish1((std::thread(thread_finish)));
    std::cout<<"test begin\n";
}

void condition_test(){
    boost::format fmt{"the condition test is on, wait for something\n"},fmt1{"the x has got, and x is %1%\n"};
    std::cout<<fmt;
    std::unique_lock<std::mutex> lk(mut_);  // 4
    data_cond.wait(
         lk,[]{return x>=20;});
    fmt1%x;
    lk.unlock();
    for(int i=0;i<1000;++i);
    lk.lock();
    std::cout<<fmt.str()+fmt1.str()<<fmt1%x;
    
    

}
int threadFunction() {
    std::cout << "Thread function is running" << std::endl;
    return x;
}

int main(int argc,char**argv){
    if(argc>2||argc<2){

        return -1;
    }
    if(std::atoi(argv[argc-1])){
        mutex_flag=true;
        std::cout<<"flag on"<<"\n";
    }


    test();
    // std::cout<<"test finish\n";
    auto deferredThread =std::async(std::launch::deferred,threadFunction);
    // scoped_thread defer_finish2{std::thread{std::thread{thread_finish},std::launch::deferred}};

    //scoped_thread finish2((std::thread(thread_finish)));
    // 临时对象的问题?
    // scoped_thread finish2(std::thread(thread_finish));对于临时对象使用了隐式转换,但是scoped禁止隐式
    // 搞了半天教程错了
    //搞不懂显示还是隐式
    //列表初始化(()) {}这两个是等价的
    //列表初始化为显示,直接初始化为隐式
    // std::thread test(thread_finish);
    // scoped_thread f(std::move(test));
    std::cout<<" 可支持的线程数"<<std::thread::hardware_concurrency()<<"\n";
    std::vector<std::thread> thread_v;
    std::thread cond_thr(condition_test);
    cond_thr.detach();

    for(int i=0;i<100;++i){
        std::thread t1(thread_do_something,std::ref(i));
        t1.detach();
        // t1.join();
        
        //thread_v.push_back(std::thread(thread_do_something,std::ref(i)));
    }
    //std::thread t1(thread_do_something,1);
    // std::thread t2(thread_do_something,2);
    // t1.join();
    // t2.join();
    //std::for_each(thread_v.begin(),thread_v.end(),std::mem_fn(&std::thread::detach));
//     while(1){
// //防止程序过早休眠
//     }
    for(int i=0;i<1000000000;++i);


    std::cout<<deferredThread.get()<<std::endl;

    // auto ret=deferredThread.get();
    // std::cout<<ret<<std::endl;

    return 0;
}