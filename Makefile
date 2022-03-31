CXX = g++
CXXFLAGS = -Wall -g

podiceps: PD_File.o main.o
	$(CXX) $^ -o $@

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~ deps.mk snake

deps.mk: $(wildcard *.cpp) Makefile
	$(CXX) -MM $(wildcard *.cxx) > $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif
