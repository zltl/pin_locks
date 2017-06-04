#ifndef __LOCK_GRAPH_H__
#define __LOCK_GRAPH_H__

#include <map>
#include <vector>

#define UL unsigned long

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
private:

	bool CheckDeadLock(int u);

	bool dfs(int u, std::vector<bool> flags);

	UL GetTidMin(UL tid);
	UL GetMidMin(UL mid);

	int AddEdge(int u, int v);
	int DeleteEdge(int u, int v);
	int EnsureNode(int u);
	int EnsureM2t(int m);

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
