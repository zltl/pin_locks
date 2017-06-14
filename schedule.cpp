#include "schedule.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>


Schedule::Schedule() {
	running = -1;
	PIN_MutexInit(&mu);
	srand((unsigned int)time(NULL));
}

void Schedule::StartMaxPrio() {
	std::cout << "StartMaxPrio() " << std::endl;
	PIN_MutexLock(&mu);
	map<UL, UL>::reverse_iterator it = p2t.rbegin();

	if (it != p2t.rend()) {
		UL tid = it->second;
		running = tid;
		p2t.erase(it->first);
		std::cout << "start " << tid << std::endl;
		map<UL, UL>::iterator it2;
		UL r = rand() % PRIORITY_MAX + PRIORITY_LEVEL1_ADD;
		while ((it2 = p2t.find(r)) != p2t.end()) {
			r = rand() + 1;
			r = rand() % PRIORITY_MAX + PRIORITY_LEVEL1_ADD;
		}

		std::cout << "StartMaxPrio() set" << tid << " -> " << r << std::endl;
		p2t[r] = tid;
		t2p[tid] = r;

		PIN_SemaphoreSet(&t2s[tid]);
	} else {
		running = -1;
		std::cout << "StartMaxPrio start nothing" << std::endl;
	}
	PIN_MutexUnlock(&mu);
}

bool Schedule::Choose(UL tid) {
	StartMaxPrio();

	PIN_MutexLock(&mu);
	std::cout << "Choose locked mu()" << std::endl;
	PIN_SEMAPHORE *s = &t2s[tid];
	PIN_MutexUnlock(&mu);

	std::cout << "Choose wait " << tid << std::endl;
	PIN_SemaphoreWait(s);

	return true;
}

void Schedule::ThreadStart(UL tid) {
	PIN_MutexLock(&mu);
	std::cout << "ThreadStart " << tid << std::endl;
	UL r = rand() % PRIORITY_MAX + PRIORITY_LEVEL2_ADD;

	map<UL, UL>::iterator it;
	while ((it = p2t.find(r)) != p2t.end()) {
		r = rand() % PRIORITY_MAX + PRIORITY_LEVEL2_ADD;
	}

	p2t[r] = tid;
	t2p[tid] = r;
	std::cout << "ThreadStart() set" << tid << " -> " << r << std::endl;

	t2s[tid] = PIN_SEMAPHORE();
	PIN_SEMAPHORE *s = &t2s[tid];

	PIN_SemaphoreInit(s);

	PIN_MutexUnlock(&mu);

	// std::cout << "ThreadStart wait " << tid << std::endl;
	// PIN_SemaphoreWait(s);
}

void Schedule::ThreadEnd(UL tid) {
	PIN_MutexLock(&mu);
	std::cout << "ThreadEnd " << tid << std::endl;
	t2p.erase(tid);
	map<UL, UL>::iterator it;
	for (it = p2t.begin(); it != p2t.end(); ) {
		if (it->second == tid) {
			p2t.erase(it++);
		} else {
			it++;
		}
	}

	PIN_SemaphoreFini(&t2s[tid]);
	running = -1;

	t2s.erase(tid);
	PIN_MutexUnlock(&mu);

	StartMaxPrio();
}

