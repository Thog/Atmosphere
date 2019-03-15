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
#include "usb_serial_interface.hpp"

enum SerialCmd {
    Serial_Cmd_BindInterface = 0,
};

class UsbSerialService final : public IServiceObject {
   private:
    /* Actual commands. */
    Result BindInterface(Out<std::shared_ptr<SerialInterface>> a_OutInterface,
                         s32 a_InterfaceId);

   public:
    DEFINE_SERVICE_DISPATCH_TABLE{
        MakeServiceCommandMeta<Serial_Cmd_BindInterface,
                               &UsbSerialService::BindInterface>(),
    };
};
