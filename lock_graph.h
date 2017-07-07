#ifndef __LOCK_GRAPH_H__
#define __LOCK_GRAPH_H__

#include <map>
#include <vector>
#include <string>
#include "tools.h"

class LockGraph {
public:
	enum {
		LOCK_SUCCESS,
		LOCK_OTHER_HOLD,
		LOCK_DEAD_LOCK,
		LOCK_UNLOCK_UNLOCK
	};

	LockGraph();
	~LockGraph();

	int Lock(UL tid, UL maddr);
	int Unlock(UL tid, UL maddr);

	void draw(std::string path);
private:

	bool CheckDeadLock(int t);
	int dlt;

	UL GetTidMin(UL tid);
	UL GetMidMin(UL mid);

	int EnsureM2t(int m);
	int EnsureT2m(int t);

	std::map<UL, int> tid2min;
	std::map<int, UL> min2tid;
	int tidiota;

	std::map<UL, int> mid2min;
	std::map<int, UL> min2mid;
	int midiota;

	// mutex owner thread
	std::vector<int> m2t;
	// thread want mutex
	std::vector<int> t2m;
};

#endif /* __LOCK_GRAPH_H__ */
