CXX = g++
CXXFLAGS = -std=c++20 -Wall
LDFLAGS = -lcurl -ljson-c

SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

dgnsu: $(OBJS)
	$(CXX) $(CXXFLAGS) -o dgnsu $(OBJS) $(LDFLAGS)

%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -Wall -Wno-unused -Wno-sign-compare -Wno-reorder -Wno-comment -c -o $@ $<
