ALL: mutex dead

CPPFLAGS=-g -pthread

mutex: mutex.cpp
	$(CXX) $(CPPFLAGS) mutex.cpp -o mutex
dead: dead.cpp
	$(CXX) $(CPPFLAGS) dead.cpp -o dead

.PHONY: clean

clean:
	rm -f *.o mutex dead
