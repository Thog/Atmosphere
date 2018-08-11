#include <switch.H>

Result fsMountBis(Service* s, u32 partition_id, FsFileSystem* fs_out)
{
    Result rc;

    char buf[300];
    buf[0] = '\0';

    IpcCommand c;
    ipcInitialize(&c);
    ipcAddSendStatic(&c, buf, sizeof buf, 0);

    struct {
        u64 magic;
        u64 cmd_id;
        u64 partition_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 11;
    raw->partition_id = partition_id;

    rc = serviceIpcDispatch(s);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc)) {
            serviceCreate(&fs_out->s, r.Handles[0]);
        }
    }

    return rc;
}


Result fsDevMountPartition(const char *device_name, u32 partition_id)
{
    FsFileSystem fs;
    Result rc = fsMountBis(fsGetServiceSession(), partition_id, &fs);
    if (R_SUCCEEDED(rc))
    {
        int mount_res = fsdevMountDevice(device_name, fs);
        if (mount_res == -1)
            return MAKERESULT(Module_Libnx, LibnxError_InitFail_FS);
    }
    return rc;
}