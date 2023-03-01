CXX = g++
CXXFLAGS = -Wall -g

#Use FLTK
LFLAGS = -lfltk

#Unit tests(CppUTest)
UTESTS ?= yes
ifeq '$(UTESTS)' 'yes'
UTFLAGS = -D RUN_UTESTS=1
LFLAGS += -lCppUTest
else
UTFLAGS = -D RUN_UTESTS=0
endif

podiceps-fltk: DError.o DFile.o DHandler.o DGraph.o main.o
	$(CXX) $^ -o $@ $(LFLAGS)

%.o: %.cxx
	$(CXX) $(UTFLAGS) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~ deps.mk podiceps-fltk

deps.mk: $(wildcard *.cxx) Makefile
	$(CXX) -MM $(wildcard *.cxx) > $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif
