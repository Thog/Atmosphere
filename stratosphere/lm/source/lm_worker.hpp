#pragma once
#include <switch.h>
#include <stratosphere.hpp>

// TODO: find a less strange way of doing this? (This is actually the same code as FsMitM worker)
class LmWorker {
    private:
        static Result AddWaitableCallback(void *arg, Handle *handles, size_t num_handles, u64 timeout);
    public:
        static void Main(void *arg);
        static void AddWaitable(IWaitable *waitable);
};