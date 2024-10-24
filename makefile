CXX := g++
CXXFLAGS := -I/usr/include/boost -I./include
LDFLAGS := -L/usr/lib/x86_64-linux-gnu/ -pthread -std=c++11

LDFLAGS+= -Wp,-MD

target := thread.o 
#thread.o
-include $(wildcard *.d)

build: $(target)
	$(CXX) -o build $(target) $(CXXFLAGS) $(LDFLAGS) && ./build 1

%.o: %.cpp
	$(CXX) -c -g -o  $@ $< $(CXXFLAGS)
#需要添加一个gdb选项
# %.o : %.c
# gcc -Wp,-MD,.$@.d  -c -o $@  $<
gdb: $(target)
	$(CXX) -g -o build $(target) $(CXXFLAGS) $(LDFLAGS)
#boost 导致gdb看不到源码,排除了本地环境和pthread
#好像也不是boost,直接用g++的也可以看
#懂了 .o文件生成时没有-g

clean:
	rm -f *.o build *.d

.PHONY: clean