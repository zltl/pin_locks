#include "schedule.h"
#include <stdlib.h>
#include <iostream>

bool Schedule::Choose(UL tid) {
	std::cout << "Choose()" << std::endl;
	map<UL, UL>::reverse_iterator it = p2t.rbegin();

	if (it != p2t.rend()) {
		if (tid == it->second) {
			p2t.erase(it->first);
			map<UL, UL>::iterator it2;
			UL r = rand() + 1;
			while ((it2 = p2t.find(r)) != p2t.end()) {
				r = rand() + 1;
			}

			p2t[r] = tid;
			t2p[tid] = r;
			return true;
		} else {
			return false;
		}
	}
	return true;
}

void Schedule::ThreadStart(UL tid) {
	UL r = rand() + 1;

	map<UL, UL>::iterator it;
	while ((it = p2t.find(r)) != p2t.end()) {
		r = rand() + 1;
	}

	p2t[r] = tid;
	t2p[tid] = r;
}

void Schedule::ThreadEnd(UL tid) {
	t2p.erase(tid);
	map<UL, UL>::iterator it;
	for (it = p2t.begin(); it != p2t.end(); ) {
		if (it->second == tid) {
			p2t.erase(it++);
		} else {
			it++;
		}
	}
}

