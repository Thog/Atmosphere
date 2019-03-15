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

#include "usb_serial_interface.hpp"
#include "serial.hpp"

SerialInterface::~SerialInterface() {
    UsbSerial::Get()->UnBind(this->m_InterfaceId);
}

Result SerialInterface::Read(OutBuffer<u8> a_OutBuffer, Out<u64> out_read) {
    u64 bytes_read;

    Result res = UsbSerial::Get()->Read(this->m_InterfaceId, a_OutBuffer.buffer,
                                        a_OutBuffer.num_elements, &bytes_read);

    if (R_SUCCEEDED(res)) {
        out_read.SetValue(bytes_read);
        return 0;
    }

    return res;
}

Result SerialInterface::Write(InBuffer<u8> a_InBuffer, Out<u64> out_written) {
    u64 bytes_written;

    Result res =
        UsbSerial::Get()->Write(this->m_InterfaceId, a_InBuffer.buffer,
                                a_InBuffer.num_elements, &bytes_written);

    if (R_SUCCEEDED(res)) {
        out_written.SetValue(bytes_written);
        return 0;
    }

    return res;
}