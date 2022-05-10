CXX = g++
CXXFLAGS = -Wall -g

podiceps: DError.o DFile.o DHandler.o DGraph.o main.o -lfltk
	$(CXX) $^ -o $@

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~ deps.mk podiceps

deps.mk: $(wildcard *.cxx) Makefile
	$(CXX) -MM $(wildcard *.cxx) > $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif
