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

enum SerialInterfaceCmd {
    SerialInterface_Cmd_Read = 0,
    SerialInterface_Cmd_Write = 1,
};

class SerialInterface final : public IServiceObject {
   private:
    int m_InterfaceId;
    /* Actual commands. */
    Result Read(OutBuffer<u8> a_OutBuffer, Out<u64> out_read);
    Result Write(InBuffer<u8> a_InBuffer, Out<u64> out_written);

   public:
    SerialInterface(int a_InterfaceId) : m_InterfaceId(a_InterfaceId) {}
    ~SerialInterface();
    DEFINE_SERVICE_DISPATCH_TABLE{
        MakeServiceCommandMeta<SerialInterface_Cmd_Read,
                               &SerialInterface::Read>(),
        MakeServiceCommandMeta<SerialInterface_Cmd_Write,
                               &SerialInterface::Write>(),
    };
};
