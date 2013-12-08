CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic
TARGET=tiling

SRC=tiling.cpp neuron.cpp testset.cpp constant.cpp
OBJS=$(SRC:.cpp=.o)
MAIN=$(TARGET)

.PHONY: depend clean

all: $(MAIN)
$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend:
	makedepend $(INCLUDES) $^
# DO NOT DELETE
