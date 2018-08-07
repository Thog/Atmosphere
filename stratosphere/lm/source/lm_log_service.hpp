#pragma once
#include <switch.h>
#include <stratosphere/iserviceobject.hpp>
#include <stratosphere.hpp>
#include "lm_logger.hpp"

enum LogServiceServiceCmd {
    Lm_Cmd_Initialize = 0,
};

class LogService final : public IServiceObject {
    public:
        Result dispatch(IpcParsedCommand &r, IpcCommand &out_c, u64 cmd_id, u8 *pointer_buffer, size_t pointer_buffer_size) override;
        Result handle_deferred() override {
            // never deferred?
            return 0;
        }

        LogService *clone() override {
            return new LogService();
        }
    private:
        std::tuple<Result, OutSession<Logger>> Initialize(PidDescriptor pid, u64 unk);
};
