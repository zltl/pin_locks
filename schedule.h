#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <map>
#include "tools.h"
#include "pin.H"

using std::map;

class Schedule {
	public:
		Schedule();
		~Schedule() {}

		bool Choose(UL tid);
		void ThreadStart(UL tid);
		void ThreadEnd(UL tid);

		void StartMaxPrio();
	private:
		map <UL, UL> t2p;
		map <UL, UL> p2t;

		map <UL, PIN_SEMAPHORE> t2s;
		long running;
		PIN_MUTEX mu;

		enum {
			PRIORITY_MAX = 0x00FFFFFF,
			PRIORITY_LEVEL1_ADD = 0x10000000,
			PRIORITY_LEVEL2_ADD = 0x20000000
		};
};

#endif /* __SCHEDULE_H__ */
