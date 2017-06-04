#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include "pin.H"
#include "lock_graph.h"

string invalid = "invalid_rtn";

LockGraph lg;

const string *Target2String(ADDRINT target) {
	string name = RTN_FindNameByAddress(target);
	if (name == "")
		return &invalid;
	else
		return new string(name);
}

static PIN_MUTEX printm;
static PIN_MUTEX check_mutex;

void InitTrace() {
	PIN_MutexInit(&printm);
	PIN_MutexInit(&check_mutex);
}

VOID do_call_args(const string *s, ADDRINT arg0) {
	PIN_MutexLock(&printm);

	OS_THREAD_ID id = PIN_GetTid();
	std::cout << id << " " << *s << "(" << arg0 << ",...)" << endl;

	PIN_MutexUnlock(&printm);
}

VOID do_call_args_indirect(ADDRINT target, BOOL taken, ADDRINT arg0) {
	if (!taken) return;

	const string *s = Target2String(target);
	do_call_args(s, arg0);

	if (s != &invalid)
		delete s;
}

VOID Trace(TRACE trace, VOID *v) {
	for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl)) {
		INS tail = BBL_InsTail(bbl);
		if (INS_IsCall(tail)) {
			if (INS_IsDirectBranchOrCall(tail)) {
				const ADDRINT target = INS_DirectBranchOrCallTargetAddress(tail);
				INS_InsertPredicatedCall(tail, IPOINT_BEFORE, AFUNPTR(do_call_args),
						IARG_PTR, Target2String(target), IARG_G_ARG0_CALLER, IARG_END);
			} else {
				INS_InsertCall(tail, IPOINT_BEFORE, AFUNPTR(do_call_args_indirect),
						IARG_BRANCH_TARGET_ADDR, IARG_BRANCH_TAKEN, IARG_G_ARG0_CALLER, IARG_END);
			}
		} else {
			// sometimes code is not in an image
			RTN rtn = TRACE_Rtn(trace);

			// also track stup jumps into share libraries
			if (RTN_Valid(rtn) && !INS_IsDirectBranchOrCall(tail) && ".plt" == SEC_Name(RTN_Sec( rtn))) {
				INS_InsertCall(tail, IPOINT_BEFORE, AFUNPTR(do_call_args_indirect),
						IARG_BRANCH_TARGET_ADDR, IARG_BRANCH_TAKEN,  IARG_G_ARG0_CALLER, IARG_END);
			}
		}
	}
}

int my_lock(CONTEXT* ctxt, AFUNPTR pf_lock, pthread_mutex_t *m) {
	OS_THREAD_ID id = PIN_GetTid();
	std::cout << id << " my_lock( " << m << " )"<< std::endl;
	while (1) {
		PIN_MutexLock(&check_mutex);

		// TODO schedule
		int r = lg.Lock(id, (unsigned long)m);
		if (r == LockGraph::LOCK_DEAD_LOCK) {
			std::cout << "DEAD_LOCK" << std::endl;
			PIN_MutexUnlock(&check_mutex);
			break;
		} else if (r == LockGraph::LOCK_SUCCESS) {
			PIN_MutexUnlock(&check_mutex);
			break;
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

