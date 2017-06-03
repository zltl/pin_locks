#include "lock_graph.h"
#include <map>

using std::vector;

LockGraph::LockGraph() {
	tidiota = 1;
	midiota = 1;
}

LockGraph::~LockGraph() { }

int LockGraph::Lock(UL tid, UL maddr) {
	int t = GetTidMin(tid), m = GetMidMin(maddr);
}

int LockGraph::Unlock(UL tid, UL maddr) {

}

int LockGraph::AddEdge(int u, int v) {
	EnsureNode(u);
	EnsureNode(v);
	edge[u].push_back(v);
}

int LockGraph::DeleteEdge(int u, int v) {
	if (edge.size() < u || edge.size() < v) {
		return 0;
	}
	for (vector<int>::iterator iter = edge[u].begin(); iter != edge[u].end(); iter++) {
		if (*iter == v) {
			edge[u].erase(iter);
			// no duplicate edge
			return 0;
		}
	}
	return 0;
}

int LockGraph::EnsureNode(int u) {
	// id begin with 0, so <=, not <
	while (edge.size() <= u) {
		edge.push_back(vector<int>());
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
