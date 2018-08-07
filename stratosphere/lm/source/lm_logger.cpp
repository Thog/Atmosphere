#include "lm_logger.hpp"
#include "log_utils.h"

Result Logger::dispatch(IpcParsedCommand &r, IpcCommand &out_c, u64 cmd_id, u8 *pointer_buffer, size_t pointer_buffer_size) {
    Result rc = 0xF601;

    switch ((LoggerServiceCmd)cmd_id) {
        case Lm_Cmd_Log:
            log_printf("Lm_Cmd_Log\n");
            rc = WrapIpcCommandImpl<&Logger::Log>(this, r, out_c, pointer_buffer, pointer_buffer_size);
            log_printf("RC: 0x%08X\n", rc);

            if (R_FAILED(rc))
                rc = 0xF601;
            break;
        case Lm_Cmd_SetDestination:
            rc = WrapIpcCommandImpl<&Logger::SetDestination>(this, r, out_c, pointer_buffer, pointer_buffer_size);
            break;
        default:
            break;
    }
    return rc;
}

std::tuple<Result> Logger::Log(InSmartBuffer<char> logBuffer) {
    // stub
    return {0};
}

std::tuple<Result> Logger::SetDestination(u64 destination) {
    // stub
    return {0};
}