#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include "pin.H"
#include "stdio.h"

VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 code, VOID *v);
VOID ThreadStart(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID *v);

#endif /* __SCHEDULE_H__ */
