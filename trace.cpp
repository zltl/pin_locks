#include <unistd.h>
#include <iostream>
#include "pin.H"

string invalid = "invalid_rtn";

const string *Target2String(ADDRINT target) {
	string name = RTN_FindNameByAddress(target);
	if (name == "")
		return &invalid;
	else
		return new string(name);
}

static PIN_MUTEX trace_mutex;

void InitTrace() {
	PIN_MutexInit(&trace_mutex);
}

VOID do_call_args(const string *s, ADDRINT arg0) {
	PIN_MutexLock(&trace_mutex);

	OS_THREAD_ID id = PIN_GetTid();
	std::cout << id << " " << *s << "(" << arg0 << ",...)" << endl;

	PIN_MutexUnlock(&trace_mutex);
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

