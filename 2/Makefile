PYTHIA8DIR = ../1/pythia8215

SOURCE = $(wildcard *.cpp)
OBJS = $(addsuffix .o,$(basename $(SOURCE)))

CFLAGS = -Wall -Wextra --std=c++11 -O2 -march=native -m64 -fPIC -I$(PYTHIA8DIR)/include `root-config --cflags`
LFLAGS = $(PYTHIA8DIR)/lib/libpythia8.a -Wl,-rpath $(PYTHIA8DIR)/lib -ldl `root-config --glibs --ldflags`

BIN = myHiggs
DCT = signalDct_rdict.pcm

GARBAGE = $(wildcard $(OBJS) $(BIN) *out)


.cpp.o:
	$(CXX) $(CFLAGS) -c $^ -o $@


all: $(PYTHIA8DIR)
	@echo wololo
	@$(MAKE) $(DCT)
	@$(MAKE) $(BIN)

$(DCT):
	@ln -s ../1/$@ $@

$(BIN): $(OBJS)
	$(CXX) ../1/signalDct.cpp $^ -w $(CFLAGS) $(LFLAGS) -o $@ 

clean:
	rm -rf $(GARBAGE)
