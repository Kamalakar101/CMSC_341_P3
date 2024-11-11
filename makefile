CXX = g++
CXXFLAGS = -Wall -g

proj3: mqueue.o driver.cpp
		$(CXX) $(CXXFLAGS) mqueue.o driver.cpp -o proj3

mqueue.o: mqueue.cpp  mqueue.h
		$(CXX) $(CXXFLAGS) -c mqueue.cpp

run:
		./proj3

val:
		valgrind ./proj3

##Use this to clean out your directory
clean:
		rm *~
		rm *.

##Use this when you want to debug your whole project
debug1:
		gdb gdbarg1 --args ./proj3

##Use this when you want to submit your files (from your project directory)
submit:
		cp mqueue.h mqueue.cpp mytest.cpp ~/cs341proj/proj3