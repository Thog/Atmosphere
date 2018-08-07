#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <stdarg.h>

#include <switch.h>
#include <stratosphere.hpp>

#include "fatal_utils.h"
#include "lm_worker.hpp"
#include "lm_log_service.hpp"
#include "log_utils.h"

#define PMC_PHYS_ADDR 0x7000E400

extern "C" {
    extern u32 __start__;

    u32 __nx_applet_type = AppletType_None;

    #define INNER_HEAP_SIZE 0x200000
    size_t nx_inner_heap_size = INNER_HEAP_SIZE;
    char   nx_inner_heap[INNER_HEAP_SIZE];
    
    void __libnx_initheap(void);
    void __appInit(void);
    void __appExit(void);
}

void __libnx_initheap(void) {
	void*  addr = nx_inner_heap;
	size_t size = nx_inner_heap_size;

	/* Newlib */
	extern char* fake_heap_start;
	extern char* fake_heap_end;

	fake_heap_start = (char*)addr;
	fake_heap_end   = (char*)addr + size;
}

void __appInit(void) {
    Result rc;

    rc = smInitialize();
    if (R_FAILED(rc)) {
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));
    }

    rc = fsInitialize();
    if (R_FAILED(rc)) {
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
    }
}

void __appExit(void) {
    smExit();
}

int main(int argc, char **argv) {
    Thread worker_thread = {0};

    consoleDebugInit(debugDevice_SVC);

    //svcSleepThread(2000000000L);
    Result res = threadCreate(&worker_thread, &LmWorker::Main, NULL, 0x20000, 45, svcGetCurrentProcessorNumber());
    if (R_FAILED(res)) {
        log_printf("Cannot create Thread: 0x%08X\n", res);
        return 0;
    }
    res = threadStart(&worker_thread);
    if (R_FAILED(res)) {
        log_printf("Cannot start Thread: 0x%08X\n", res);
        return 0;
    }

    log_printf("Starting server manager\n");
    auto server_manager = std::make_unique<WaitableManager>(U64_MAX);
    server_manager->add_waitable(new ServiceServer<LogService>("lm", 0x20));
    log_printf("Starting processing...\n");

    server_manager->process();
    return 0;
}