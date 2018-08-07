#include "lm_log_service.hpp"
#include "lm_worker.hpp"

Result LogService::dispatch(IpcParsedCommand &r, IpcCommand &out_c, u64 cmd_id, u8 *pointer_buffer, size_t pointer_buffer_size) {
    Result rc = 0xF601;

    switch ((LogServiceServiceCmd)cmd_id) {
        case Lm_Cmd_Initialize:
            rc = WrapIpcCommandImpl<&LogService::Initialize>(this, r, out_c, pointer_buffer, pointer_buffer_size);
            break;
        default:
            break;
    }
    return rc;
}

std::tuple<Result, OutSession<Logger>> LogService::Initialize(PidDescriptor pid, u64 unk) {
    std::shared_ptr<Logger> logger = std::make_shared<Logger>();
    IPCSession<Logger> *ipc_session = new IPCSession<Logger>(logger);
    if (this->get_owner() == nullptr)
        LmWorker::AddWaitable(ipc_session);
    OutSession out_session = OutSession(ipc_session);
    return {0, out_session};
}