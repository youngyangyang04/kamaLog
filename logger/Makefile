#编译器和编译选项
CXX=g++
#DEBUG_FLAGS= -O0 -g
CXXFLAGS = -std=c++11 -Wall 
#链接库
LIBS= -lpthread


#源文件目录
SRCDIR= .

#获取源文件
SRCS = $(wildcard $(SRCDIR)/*.cc)

#目标文件
SRCS_OBJS=$(patsubst %.cc, %.o, $(SRCS))

#默认目标
all:test

#链接可执行文件
test:$(SRCS_OBJS)
	$(CXX) $(SRCS_OBJS) $(LIBS) -o $@

#编译规则
%.o:%.cc
	$(CXX) -c $< $(CXXFLAGS) -o $@

# 清理生成文件
clean:
	@rm -rf $(SRCS_OBJS) test
# 伪目标
.PHONY: all clean