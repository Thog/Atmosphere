#pragma once
#include <switch.h>
#include <stratosphere/iserviceobject.hpp>
#include <stratosphere.hpp>

enum LoggerServiceCmd {
    Lm_Cmd_Log = 0,
    Lm_Cmd_SetDestination = 1
};

class Logger final : public IServiceObject {
    public:
        Result dispatch(IpcParsedCommand &r, IpcCommand &out_c, u64 cmd_id, u8 *pointer_buffer, size_t pointer_buffer_size) override;
        Result handle_deferred() override {
            // never deferred?
            return 0;
        }

        Logger *clone() override {
            return new Logger();
        }
    private:
        std::tuple<Result> Log(InSmartBuffer<char> logBuffer);
        std::tuple<Result> SetDestination(u64 destination);
};
