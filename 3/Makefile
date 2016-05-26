SOURCE = $(wildcard *.cpp)
OBJS = $(addsuffix .o,$(basename $(SOURCE)))

CFLAGS = -Wall -Wextra --std=c++11 -O2 -march=native -m64 -fPIC `root-config --cflags`
LFLAGS = `root-config --glibs --ldflags`

BIN = histoHiggs
PDF = FatHiggs.pdf

GARBAGE = $(wildcard $(OBJS) $(BIN) $(PDF))


.cpp.o:
	$(CXX) $(CFLAGS) -c $^ -o $@


all:
	@$(MAKE) $(BIN)
	@$(MAKE) $(PDF)


$(BIN): $(OBJS)
	$(CXX) $^ $(LFLAGS) -o $@ 

$(PDF): $(BIN)
	./$(BIN)
pdf:
	@$(MAKE) $(PDF)

clean:
	rm -rf $(GARBAGE)
