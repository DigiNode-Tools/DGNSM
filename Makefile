CXXFLAGS = -O3 -g0
LDFLAGS = $(CXXFLAGS)

digibyte-connect:  main.o
	g++ -pthread $(LDFLAGS) -o digibyte-connect main.o

%.o: %.cpp *.h
	g++ -std=c++11 -pthread $(CXXFLAGS) -Wall -Wno-unused -Wno-sign-compare -Wno-reorder -Wno-comment -c -o $@ $<
