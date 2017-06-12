#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include "pin.H"
#include "lock_graph.h"
#include "schedule.h"

static PIN_MUTEX check_mutex;
LockGraph lg;
static Schedule sd;

void InitReplace() {
	PIN_MutexInit(&check_mutex);
}

int my_lock(CONTEXT* ctxt, AFUNPTR pf_lock, pthread_mutex_t *m) {
	OS_THREAD_ID id = PIN_GetTid();
	std::cout << id << " my_lock( " << m << " )"<< std::endl;
	while (1) {
		PIN_MutexLock(&check_mutex);

		if (sd.Choose(id)) {
			int r = lg.Lock(id, (unsigned long)m);
			if (r == LockGraph::LOCK_DEAD_LOCK) {
				std::cout << "DEAD_LOCK" << std::endl;
				PIN_MutexUnlock(&check_mutex);
				break;
			} else if (r == LockGraph::LOCK_SUCCESS) {
				PIN_MutexUnlock(&check_mutex);
				break;
			}
		}

		PIN_MutexUnlock(&check_mutex);
	}

	int res = 0;
	PIN_CallApplicationFunction(ctxt, PIN_ThreadId(),
			CALLINGSTD_DEFAULT, pf_lock, NULL,
			PIN_PARG(int), &res,
			PIN_PARG(pthread_mutex_t*), m,
			PIN_PARG_END());

	return res;
}


int my_unlock(CONTEXT* ctxt, AFUNPTR pf_unlock, pthread_mutex_t *m) {
	PIN_MutexLock(&check_mutex);

	OS_THREAD_ID id = PIN_GetTid();

	std::cout << id << " my_unlock( " << m << " )"<< std::endl;

	int r = lg.Unlock(id, (unsigned long)m);
	if (r == LockGraph::LOCK_UNLOCK_UNLOCK) {
		std::cout << "LOCK_UNLOCK_UNLOCK : " << m << std::endl;
	}

	int res = 0;
	PIN_CallApplicationFunction(ctxt, PIN_ThreadId(),
			CALLINGSTD_DEFAULT, pf_unlock, NULL,
			PIN_PARG(int), &res,
			PIN_PARG(pthread_mutex_t*), m,
			PIN_PARG_END());

	PIN_MutexUnlock(&check_mutex);
	return res;
}

VOID ImageLoad(IMG img, VOID *v) {
	RTN rtn = RTN_FindByName(img, "pthread_mutex_lock");
	if (RTN_Valid(rtn)) {
		PROTO proto_lock = PROTO_Allocate(PIN_PARG(int),
				CALLINGSTD_DEFAULT, "pthread_mutex_lock",
				PIN_PARG(pthread_mutex_t*), PIN_PARG_END());

		RTN_ReplaceSignature(rtn, AFUNPTR(my_lock),
				IARG_PROTOTYPE, proto_lock, IARG_CONST_CONTEXT,
				IARG_ORIG_FUNCPTR, IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
				IARG_END);
		PROTO_Free(proto_lock);
	}

	rtn = RTN_FindByName(img, "pthread_mutex_unlock");
	if (RTN_Valid(rtn)) {
		PROTO proto_unlock = PROTO_Allocate(PIN_PARG(int),
				CALLINGSTD_DEFAULT, "pthread_mutex_unlock",
				PIN_PARG(pthread_mutex_t*), PIN_PARG_END());

		RTN_ReplaceSignature(rtn, AFUNPTR(my_unlock),
				IARG_PROTOTYPE, proto_unlock, IARG_CONST_CONTEXT,
				IARG_ORIG_FUNCPTR, IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
				IARG_END);
	}
}

VOID ThreadStart(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID *v) {
	PIN_MutexLock(&check_mutex);
	sd.ThreadStart(PIN_GetTid());
	PIN_MutexUnlock(&check_mutex);
}

VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 code, VOID *v) {
	PIN_MutexLock(&check_mutex);
	sd.ThreadEnd(PIN_GetTid());
	PIN_MutexUnlock(&check_mutex);
}

