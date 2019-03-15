/*
 * Copyright (c) 2019 Thog
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <malloc.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <atmosphere.h>
#include <switch.h>
#include <stratosphere.hpp>

#include "serial.hpp"
#include "usb_serial_service.hpp"

extern "C" {
extern u32 __start__;

u32 __nx_applet_type = AppletType_None;

#define INNER_HEAP_SIZE 0x100000
size_t nx_inner_heap_size = INNER_HEAP_SIZE;
char nx_inner_heap[INNER_HEAP_SIZE];

void __libnx_initheap(void);
void __appInit(void);
void __appExit(void);
}

void __libnx_initheap(void) {
    void* addr = nx_inner_heap;
    size_t size = nx_inner_heap_size;

    /* Newlib */
    extern char* fake_heap_start;
    extern char* fake_heap_end;

    fake_heap_start = (char*)addr;
    fake_heap_end = (char*)addr + size;
}

void __appInit(void) {
    Result rc;

    SetFirmwareVersionForLibnx();

    rc = smInitialize();
    if (R_FAILED(rc)) {
        std::abort();
    }
}

void __appExit(void) {
    /* Cleanup services. */
    smExit();
}

int main(int argc, char** argv) {
    consoleDebugInit(debugDevice_SVC);

    Result res = UsbSerial::Initialize();
    if (R_FAILED(res)) {
        RebootToRcm();
    }

    auto server_manager = new WaitableManager(4);

    /* Create services. */
    server_manager->AddWaitable(
        new ServiceServer<UsbSerialService>("usb:uart", 4));

    /* Loop forever, servicing our services. */
    server_manager->Process();

    delete server_manager;

    return 0;
}
