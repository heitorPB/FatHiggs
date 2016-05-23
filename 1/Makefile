PYTHIA8URL = http://home.thep.lu.se/~torbjorn/pythia8/pythia8215.tgz 
PYTHIA8FILE = $(notdir $(PYTHIA8URL))
PYTHIA8DIR = $(basename $(PYTHIA8FILE))

SIGNAL_SOURCE = signal.cpp
SIGNAL_OBJS = $(addsuffix .o,$(basename $(SIGNAL_SOURCE)))
BACKGROUND_SOURCE = background.cpp
BACKGROUND_OBJS = $(addsuffix .o,$(basename $(BACKGROUND_SOURCE)))


GARBAGE = $(wildcard $(SIGNAL_OBJS) $(BACKGROUND_OBJS) background signal *out *pcm *.so)


CFLAGS = -I$(PYTHIA8DIR)/include -Wall -Wextra --std=c++11 -O2 -march=native -m64 -pedantic -fPIC `root-config --cflags`
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
	@rm $(PYTHIA8FILE)
	@cd $(PYTHIA8DIR) && ./configure --enable-64bit --enable-shared --cxx-common='-O2 -march=native -m64 -ansi -pedantic -W -Wall -Wshadow -fPIC' --with-root=${ROOTSYS}
	@cd $(PYTHIA8DIR) && make -j5


signal: $(SIGNAL_OBJS)
	rootcint -f $@Dct.cpp -c -I$(PYTHIA8DIR)/include PythiaRoot.h pythia8Namespace.h
	$(CXX) $^ $@Dct.cpp -o $@ -w $(CFLAGS) $(LFLAGS) 

libMyHiggs.so: signalDct.cpp $(SIGNAL_SOURCE) pythia8Namespace.h PythiaRoot.h
	g++ -shared -o $@ `root-config --ldflags` $(CFLAGS) -I$(ROOTSYS)/include $^

background: $(BACKGROUND_OBJS)
	$(CXX) $^ signalDct.cpp -w $(CFLAGS) $(LFLAGS) -o $@ 

clean:
	rm -rf $(GARBAGE)

cleanAll:
	rm -rf $(GARBAGE) $(PYTHIA8DIR) $(PYTHIA8FILE)
