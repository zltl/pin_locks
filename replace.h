#ifndef __REPLACE_H__
#define __REPLACE_H__

void InitReplace();
VOID ImageLoad(IMG img, VOID *v);

VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 code, VOID *v);
VOID ThreadStart(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID *v);

#endif /* __REPLACE_H__ */
