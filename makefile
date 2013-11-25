RM = rm -rf
O_SRCS = tsp.o 
CPP_SRCS = MyThread.cpp main.cpp threads.cpp tsp.cpp twoOpt.cpp
OBJS = MyThread.o main.o threads.o tsp.o twoOpt.o
CPP_DEPS = MyThread.d main.d threads.d tsp.d twoOpt.d 
LIBS = -lm -L/usr/include -lpthread


%.o: %.cpp
	g++ -g -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

tsp: $(OBJS)
	g++  -o "tsp" $(OBJS) $(LIBS) 

all: tsp





clean:
	rm tsp *.o *.d
	rm -f *.tour

