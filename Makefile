CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lcurl -ljson-c

DNSU:  main.o
	$(CXX) $(CXXFLAGS) -o DNSU main.o $(LDFLAGS)

%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -Wall -Wno-unused -Wno-sign-compare -Wno-reorder -Wno-comment -c -o $@ $<
