all: main.cpp 
	g++ -std=c++11 main.cpp -o main -I lib/work/mcl/include/ -I lib/work/cybozulib/include/ -I lib/work/xbyak/ -L lib/work/mcl/lib/ -lmcl -lgmp -lgmpxx -lcrypto
run: main 
	./main
clean: main 
	rm -f *~ 
	rm -f main 
