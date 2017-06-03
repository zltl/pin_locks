ALL: mutex

CPPFLAGS=-g -pthread

mutex: mutex.cpp
	$(CXX) $(CPPFLAGS) mutex.cpp -o mutex

.PHONY: clean

clean:
	rm -f mutex *.o
