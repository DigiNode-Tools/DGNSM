CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lcurl

digibyte-connect:  main.o
	$(CXX) $(CXXFLAGS) -o digibyte-connect main.o $(LDFLAGS)

%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -Wall -Wno-unused -Wno-sign-compare -Wno-reorder -Wno-comment -c -o $@ $<
