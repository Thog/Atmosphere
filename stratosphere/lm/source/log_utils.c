    #include "log_utils.h"
    
    #define LOG_BUFFER_SIZE 1024 * 1024
    static char g_log[LOG_BUFFER_SIZE] = {0};
    static size_t g_log_cur = 0;
    static bool g_has_initialized_fs_dev = false;


    void logWrite(void *data, size_t data_size)
    {
        memcpy(g_log + g_log_cur, data, data_size);
        g_log_cur += data_size;
    }

    void logFlush()
    {
        if (!g_has_initialized_fs_dev)
        {
            if (R_SUCCEEDED(fsdevMountSdmc())) {
                g_has_initialized_fs_dev = true;
            } else {
                return;
            }
        }

        if (g_log[0] != '\0') {
            FILE *runf = fopen("sdmc:/lm_debug.txt", "a");
            if (runf == NULL)
                return ;
            if (fwrite(g_log, strnlen(g_log, sizeof(g_log)), 1, runf) != 1)
                return ;
            fclose(runf);
            memset(g_log, 0, g_log_cur);
            g_log_cur = 0;
        }
    }

    void log_printf(const char* format, ...)
    {
        va_list args;

        va_start(args, format);
        g_log_cur += vsnprintf(g_log + g_log_cur, LOG_BUFFER_SIZE - g_log_cur, format, args);
        va_end(args);
        logFlush();
    }