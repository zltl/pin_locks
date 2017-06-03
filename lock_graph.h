#ifndef __LOCK_GRAPH_H__
#define __LOCK_GRAPH_H__

#include <map>
#include <vector>

#define UL unsigned long

class LockGraph {
public:
	enum {
		LOCK_SECCESS,
		LOCK_OTHER_HOLD,
		LOCK_DEAD_LOCK
	};

	LockGraph();
	~LockGraph();

	int Lock(UL tid, UL maddr);
	int Unlock(UL tid, UL maddr);
private:

	UL GetTidMin(UL tid);
	UL GetMidMin(UL mid);

	int AddEdge(int u, int v);
	int DeleteEdge(int u, int v);
	int EnsureNode(int u);

	std::map<UL, int> tid2min;
	std::map<int, UL> min2tid;
	int tidiota;

	std::map<UL, int> mid2min;
	std::map<int, UL> min2mid;
	int midiota;

	std::vector<int> m2t;
	std::vector<std::vector<int> > edge;
};

#endif /* __LOCK_GRAPH_H__ */
