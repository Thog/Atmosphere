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

#include "usb_serial_service.hpp"
#include "serial.hpp"

Result UsbSerialService::BindInterface(
    Out<std::shared_ptr<SerialInterface>> a_OutInterface, s32 a_InterfaceId) {
    bool can_bind = UsbSerial::Get()->Bind(a_InterfaceId);

    if (can_bind) {
        auto interface = std::make_shared<SerialInterface>(a_InterfaceId);
        a_OutInterface.SetValue(std::move(interface));
        return 0;
    }

    return 0xBEEF;
}