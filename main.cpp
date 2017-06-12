#include <iostream>
#include <fstream>
#include "pin.H"
#include "trace.h"
#include "replace.h"
#include "schedule.h"

INT32 Usage() {
	cerr << "This tool produces a call trace." << endl << endl;
	return -1;
}

VOID Fini(INT32 code, VOID *v) {
	std::cout << "# eof" << endl;
}

int  main(int argc, char *argv[]) {

	PIN_InitSymbols();

	if (PIN_Init(argc,argv)) {
		return Usage();
	}

	std::cout << hex;
	std::cout.setf(ios::showbase);

	string trace_header = string("#\n"
			"# Call Trace Generated By Pin\n"
			"#\n");

	std::cout.write(trace_header.c_str(),trace_header.size());

	InitTrace();
	InitReplace();

	TRACE_AddInstrumentFunction(Trace, 0);
	IMG_AddInstrumentFunction(ImageLoad, 0);

	PIN_AddThreadStartFunction(ThreadStart, 0);
	PIN_AddThreadFiniFunction(ThreadFini, 0);

	PIN_AddFiniFunction(Fini, 0);

	PIN_StartProgram();

	return 0;
}

