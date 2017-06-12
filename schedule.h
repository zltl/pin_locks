#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <map>
#include "tools.h"

using std::map;

class Schedule {
	public:
		Schedule() {}
		~Schedule() {}

		bool Choose(UL tid);
		void ThreadStart(UL tid);
		void ThreadEnd(UL tid);
	private:
		map <UL, UL> t2p;
		map <UL, UL> p2t;
};

#endif /* __SCHEDULE_H__ */
