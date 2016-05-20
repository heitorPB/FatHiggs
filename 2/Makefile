SOURCE = $(wildcard *.cpp)
OBJS = $(addsuffix .o,$(basename $(SOURCE)))

CFLAGS = -Wall -Wextra --std=c++11 -O2 -march=native -m64 -fPIC `root-config --cflags`
LFLAGS = `root-config --libs --ldflags`

BIN = myHiggs

GARBAGE = $(wildcard $(OBJS) $(bin) *out)


.cpp.o:
	$(CXX) $(CFLAGS) -c $^ -o $@


all: $(PYTHIA8DIR)
	@echo wololo
	@$(MAKE) $(BIN)

$(BIN): $(OBJS)
	$(CXX) $^ $(LFLAGS) -o $@ 

clean:
	rm -rf $(GARBAGE)
