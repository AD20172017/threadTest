#ifndef _SCOPED_THREAD_
#define _SCOPED_THREAD_

class scoped_thread
{
  std::thread t;
public:
  explicit scoped_thread(std::thread t_)
    :t(std::move(t_)){ 
        std::cout<<"init\n";
        if(!t.joinable())                                     // 2
        throw std::logic_error("No thread");
  }
  ~scoped_thread(){
    std::cout<<"destory\n";
    t.join();   
    std::cout<<"thread done\n";                                         // 3
  }
  scoped_thread(scoped_thread const&)=delete;
  scoped_thread& operator=(scoped_thread const&)=delete;
};

#endif // !_SCOPED_THREAD_
