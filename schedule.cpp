#include "schedule.h"

VOID ThreadStart(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID *v) {
	printf("thread begin %d\n",threadid);
}

VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 code, VOID *v) {
	printf("thread end %d code %d\n",threadid, code);
}

