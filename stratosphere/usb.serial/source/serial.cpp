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

#include "serial.hpp"
#include <cstdlib>
#include <cstring>
#include <mutex>

UsbSerial *UsbSerial::g_UsbSerialInstance = nullptr;

Result UsbSerial::Init() {
    std::scoped_lock<HosMutex> lock(this->m_BufferLock);
    std::scoped_lock<HosMutex> lock2(this->m_BindLock);
    UsbCommsInterfaceInfo info = {
        .bInterfaceClass = USB_CLASS_VENDOR_SPEC,
        .bInterfaceSubClass = USB_CLASS_PER_INTERFACE,
        .bInterfaceProtocol = USB_CLASS_PER_INTERFACE,
    };

    Result res = 0;

    res = usbCommsInitializeEx(this->m_InterfaceCount, &info);
    if (R_FAILED(res)) {
        goto err;
    }

    this->m_BindArray = (char *)malloc(this->m_InterfaceCount);
    if (!this->m_BindArray) {
        goto err;
    }

    memset(this->m_BindArray, 0, this->m_InterfaceCount);

    this->m_SerialBuffer = aligned_alloc(USB_SERIAL_TRANSFER_BUFFER_SIZE,
                                         USB_SERIAL_TRANSFER_BUFFER_SIZE);
    if (!this->m_SerialBuffer) {
        goto err_serial_bind;
    }

    res = svcSetMemoryAttribute(this->m_SerialBuffer,
                                USB_SERIAL_TRANSFER_BUFFER_SIZE, 8, 8);
    if (R_FAILED(res)) {
        goto err_serial_buffer;
    }

    return res;
err_serial_bind:
    free(this->m_BindArray);
    this->m_BindArray = nullptr;
err_serial_buffer:
    free(this->m_SerialBuffer);
    this->m_SerialBuffer = nullptr;
err:
    return res;
}

bool UsbSerial::Bind(int interface_id) {
    std::scoped_lock<HosMutex> lock(this->m_BindLock);

    if (interface_id < this->m_InterfaceCount &&
        this->m_BindArray[interface_id] == 0) {
        this->m_BindArray[interface_id] = 1;
        return true;
    }
    return false;
}
void UsbSerial::UnBind(int interface_id) {
    std::scoped_lock<HosMutex> lock(this->m_BindLock);
    if (interface_id < this->m_InterfaceCount) {
        this->m_BindArray[interface_id] = 0;
    }
}

UsbSerial::UsbSerial(int a_InterfaceCount)
    : m_InterfaceCount(a_InterfaceCount), m_BufferLock(), m_BindLock() {}

UsbSerial::~UsbSerial() {
    std::scoped_lock<HosMutex> lock(this->m_BufferLock);
    usbCommsExit();
    svcSetMemoryAttribute(this->m_SerialBuffer, USB_SERIAL_TRANSFER_BUFFER_SIZE,
                          0, 0);
    free(this->m_SerialBuffer);
    this->m_SerialBuffer = nullptr;
    free(this->m_BindArray);
    this->m_BindArray = nullptr;
}

Result UsbSerial::Write(int interface_id, const void *data, size_t size,
                        size_t *bytes_written) {
    std::scoped_lock<HosMutex> lock(this->m_BufferLock);
    Result res = 0;

    if (size > USB_SERIAL_TRANSFER_BUFFER_SIZE) {
        size = USB_SERIAL_TRANSFER_BUFFER_SIZE;
    }

    if (this->m_SerialBuffer == nullptr) {
        return 0xDEAD;
    }

    memcpy(this->m_SerialBuffer, data, size);

    size_t ret = usbCommsWriteEx(this->m_SerialBuffer, size, interface_id);

    if (bytes_written) {
        *bytes_written = ret;
    }

    return res;
}

Result UsbSerial::Read(int interface_id, void *data, size_t size,
                       size_t *bytes_read) {
    std::scoped_lock<HosMutex> lock(this->m_BufferLock);
    Result res = 0;

    if (size > USB_SERIAL_TRANSFER_BUFFER_SIZE) {
        size = USB_SERIAL_TRANSFER_BUFFER_SIZE;
    }

    if (this->m_SerialBuffer == nullptr) {
        return 0xDEAD;
    }

    size_t data_size = usbCommsReadEx(this->m_SerialBuffer, size, interface_id);
    memcpy(data, this->m_SerialBuffer, data_size);
    if (bytes_read) {
        *bytes_read = data_size;
    }

    return res;
}