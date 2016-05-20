PYTHIA8URL = http://home.thep.lu.se/~torbjorn/pythia8/pythia8215.tgz 
PYTHIA8FILE = $(notdir $(PYTHIA8URL))
PYTHIA8DIR = $(basename $(PYTHIA8FILE))

SIGNAL_SOURCE = signal.cpp
SIGNAL_OBJS = $(addsuffix .o,$(basename $(SIGNAL_SOURCE)))
BACKGROUND_SOURCE = background.cpp
BACKGROUND_OBJS = $(addsuffix .o,$(basename $(BACKGROUND_SOURCE)))


GARBAGE = $(wildcard $(PYTHIA8DIR) $(PYTHIA8FILE) $(SIGNAL_OBJS) $(BACKGROUND_OBJS) background signal *out)


CFLAGS = -I$(PYTHIA8DIR)/include -Wall -Wextra --std=c++11 -O2 -march=native -m64 -ansi -pedantic -fPIC `root-config --cflags`
LFLAGS = $(PYTHIA8DIR)/lib/libpythia8.a -Wl,-rpath $(PYTHIA8DIR)/lib -ldl `root-config --libs --ldflags`


.cpp.o:
	$(CXX) $(CFLAGS) -c $^ -o $@


all: $(PYTHIA8DIR)
	@echo wololo
	@$(MAKE) signal
	@$(MAKE) background


$(PYTHIA8DIR):
	@wget -nv $(PYTHIA8URL)
	@tar xf $(PYTHIA8FILE)
	@cd $(PYTHIA8DIR) && ./configure --enable-64bit --cxx-common='-O2 -march=native -m64 -ansi -pedantic -W -Wall -Wshadow -fPIC' #--with-root
	@cd $(PYTHIA8DIR) && make -j5


signal: $(SIGNAL_OBJS)
	$(CXX) $^ $(LFLAGS) -o $@ 

background: $(BACKGROUND_OBJS)
	$(CXX) $^ $(LFLAGS) -o $@ 

clean:
	rm -rf $(GARBAGE)
