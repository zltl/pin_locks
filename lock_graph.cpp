#include "lock_graph.h"
#include <map>
#include <iostream>
#include <fstream>
#include "tools.h"

using std::vector;

LockGraph::LockGraph() {
	tidiota = 1;
	midiota = 1;
	dlt = 0;
}

LockGraph::~LockGraph() { }

int LockGraph::Lock(UL tid, UL maddr) {
	int t = GetTidMin(tid), m = GetMidMin(maddr);
	EnsureM2t(m);
	EnsureT2m(t);
	if (m2t[m] != 0) {
		if (t2m[t] != m) {
			t2m[t] = m;
			if (CheckDeadLock(t)) {
				return LOCK_DEAD_LOCK;
			}
		}
		return LOCK_OTHER_HOLD;
	} else {
		m2t[m] = t;
		t2m[t] = 0;

		return LOCK_SUCCESS;
	}
}

int LockGraph::Unlock(UL tid, UL maddr) {
	int m = GetMidMin(maddr);
	if (m2t[m] == 0) {
		return LOCK_UNLOCK_UNLOCK;
	} else {
		m2t[m] = 0;
		return LOCK_SUCCESS;
	}
}

bool LockGraph::CheckDeadLock(int t) {
	int cur = t;
	while (t2m[cur] != 0 && m2t[t2m[cur]] != t) {
		cur = m2t[t2m[cur]];
	}

	return m2t[t2m[cur]] == t;
}

int LockGraph::EnsureT2m(int t) {
	// id begin with 0, so <=, not <
	while ((int)t2m.size() <= t) {
		t2m.push_back(0);
	}
	return 0;
}

int LockGraph::EnsureM2t(int m) {
	while ((int)m2t.size() <= m) {
		m2t.push_back(0);
	}
	return 0;
}

UL LockGraph::GetTidMin(UL tid) {
	UL t = tid2min[tid];
	if (t == 0) {
		t = tidiota++;
		tid2min[tid] = t;
		min2tid[t] = tid;
	}
	return t;
}

UL LockGraph::GetMidMin(UL mid) {
	UL m = mid2min[mid];
	if (m == 0) {
		m = midiota++;
		mid2min[mid] = m;
		min2mid[m] = mid;
	}
	return m;
}

void LockGraph::draw(std::string path) {
	std::fstream fs;
	fs.open(path.c_str(), std::fstream::out);

	fs <<  "digraph g {" << std::endl;
	fs << "node[shape=record, height=.1]" << std::endl;

	int cur = dlt;
	fs << "n" << cur << "[label=\"" << min2tid[cur] << "\"]" << std::endl;
	while (t2m[cur] != 0 && m2t[t2m[cur]] != dlt) {
		int p = cur;
		cur = m2t[t2m[cur]];
		fs << "n" << cur << "[label=\"" << min2tid[cur] << "\"]" << std::endl;
		fs << "n" << cur << "[label=\"" << min2tid[p] << "\"]"
			<< "->" << "n" << cur << "[label=\"" << min2tid[cur] << "\"]"
			<< "[label=\"" << min2mid[t2m[p]] << "\"]" << std::endl;
	}
	fs << "}" << std::endl;

	fs.close();
}
