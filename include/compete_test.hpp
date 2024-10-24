#ifndef _COMPETE_TEST_
#define _COMPETE_TEST_
class compete_test{
public:
    int _x;
    int &var;
    std::mutex &_mut;
    compete_test(int& x,std::mutex &mut)
        :_x(x),var(x),_mut(mut){
        
            //test
            x=-1;
            // std::random_device rd;  // 随机数设备，用于获取随机种子
            // std::default_random_engine generator(rd());
            // std::uniform_int_distribution<int> distribution(0,100);
            // int dice_roll = distribution(generator); 
            // std::cout<<"sleep"<<dice_roll<<"\n";
            if(!(random_number()%50)){
            // _mut.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout<<"sleep"<<"\n";
            }
            // sleep(100);
        }
    ~compete_test(){
        if(var!=-1){
            std::cout<<"something changed\n";
        }
        var=_x;
    }

    static int random_number(){
        std::random_device rd;  // 随机数设备，用于获取随机种子
        std::mt19937 gen(rd()); // 以随机设备生成的值作为种子
        std::uniform_int_distribution<> dis(0,100);
        return dis(gen); 
    }
};

#endif // !_COMPETE_TEST_