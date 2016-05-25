SOURCE = $(wildcard *.cpp)
OBJS = $(addsuffix .o,$(basename $(SOURCE)))

CFLAGS = -Wall -Wextra --std=c++11 -O2 -march=native -m64 -fPIC `root-config --cflags`
LFLAGS = `root-config --glibs --ldflags`

BIN = histoHiggs

GARBAGE = $(wildcard $(OBJS) $(BIN))


.cpp.o:
	$(CXX) $(CFLAGS) -c $^ -o $@


all:
	@$(MAKE) $(BIN)


$(BIN): $(OBJS)
	$(CXX) $^ $(LFLAGS) -o $@ 

clean:
	rm -rf $(GARBAGE)
