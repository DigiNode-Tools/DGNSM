CXX = g++
CXXFLAGS = -std=c++20 -Wall
LDFLAGS = -lcurl -ljson-c

dnsu:  main.o
	$(CXX) $(CXXFLAGS) -o dnsu main.o $(LDFLAGS)

%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -Wall -Wno-unused -Wno-sign-compare -Wno-reorder -Wno-comment -c -o $@ $<
