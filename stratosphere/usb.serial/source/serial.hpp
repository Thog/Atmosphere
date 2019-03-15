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

#pragma once

#include <switch.h>
#include <stratosphere.hpp>

const size_t USB_SERIAL_TRANSFER_BUFFER_SIZE = 0x4000;

class UsbSerial;

class UsbSerial {
   public:
    UsbSerial(int a_InterfaceCount);
    ~UsbSerial();

    bool Bind(int interface_id);
    void UnBind(int interface_id);
    Result Write(int interface_id, const void *data, size_t size,
                 size_t *bytes_written);
    Result Read(int interface_id, void *data, size_t size, size_t *bytes_read);

    static Result Initialize() {
        g_UsbSerialInstance = new UsbSerial(4);
        return g_UsbSerialInstance->Init();
    }
    static UsbSerial *Get() { return g_UsbSerialInstance; }

   private:
    static UsbSerial *g_UsbSerialInstance;
    Result Init();
    int m_InterfaceCount;
    void *m_SerialBuffer;
    HosMutex m_BufferLock;
    char *m_BindArray;
    HosMutex m_BindLock;
};

#define usb_log(...)                                                   \
    {                                                                  \
        char log_buf[0x200];                                           \
        snprintf(log_buf, 0x200, __VA_ARGS__);                         \
        UsbSerial::Get()->Write(0, log_buf, strlen(log_buf), nullptr); \
    }
